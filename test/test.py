from scapy.all import *
from dataset_common import find_files, read_5hp_list

import argparse
import datetime
import json
import numpy as np
import os
import time
from pathlib import Path
import torch
import torch.backends.cudnn as cudnn
from torch.utils.tensorboard import SummaryWriter

from timm.models.layers import trunc_normal_
from timm.data.mixup import Mixup
from timm.loss import LabelSmoothingCrossEntropy, SoftTargetCrossEntropy
from torchvision import datasets, transforms
import util.lr_decay as lrd
import util.misc as misc
from util.pos_embed import interpolate_pos_embed
from util.misc import NativeScalerWithGradNormCount as NativeScaler
from util.misc import count_parameters

import models_net_mamba
from contextlib import suppress
from engine import train_one_epoch

import math
import sys
from typing import Iterable, Optional
from timm.utils import accuracy
import util.lr_sched as lr_sched
from sklearn.metrics import accuracy_score, precision_recall_fscore_support, confusion_matrix
from tqdm import tqdm

from PIL import Image

def get_args_parser():
    parser = argparse.ArgumentParser('NetMamba settings', add_help=False)
    # 64
    parser.add_argument('--batch_size', default=64, type=int,
                        help='Batch size per GPU (effective batch size is batch_size * accum_iter * # gpus')
    parser.add_argument('--accum_iter', default=1, type=int,
                        help='Accumulate gradient iterations (for increasing the effective batch size under memory constraints)')

    # Model parameters
    parser.add_argument('--model', default='net_mamba_classifier', type=str, metavar='MODEL',
                        help='Name of model to train')

    parser.add_argument('--input_size', default=40, type=int,
                        help='images input size')

    parser.add_argument('--drop_path', type=float, default=0.1, metavar='PCT',
                        help='Drop path rate (default: 0.1)')

    # Optimizer parameters
    parser.add_argument('--clip_grad', type=float, default=None, metavar='NORM',
                        help='Clip gradient norm (default: None, no clipping)')
    parser.add_argument('--weight_decay', type=float, default=0.05,
                        help='weight decay (default: 0.05)')

    parser.add_argument('--lr', type=float, default=None, metavar='LR',
                        help='learning rate (absolute lr)')
    parser.add_argument('--blr', type=float, default=2e-3, metavar='LR',
                        help='base learning rate: absolute_lr = base_lr * total_batch_size / 256')
    parser.add_argument('--layer_decay', type=float, default=0.75,
                        help='layer-wise lr decay from ELECTRA/BEiT')

    # * Mixup params
    parser.add_argument('--checkpoint', default='././output/pretrain/checkpoint.pth',
                        help='checkpoint')
    parser.add_argument('--data_path', default='./ddos_datasets/flow_image', type=str,
                        help='dataset path')
    parser.add_argument('--nb_classes', default=7, type=int,
                        help='number of the classification types')
    parser.add_argument('--device', default='cuda',
                        help='device to use for training / testing')
    parser.add_argument('--seed', default=0, type=int)
    parser.add_argument('--resume', default='',
                        help='resume from checkpoint')

    parser.add_argument('--num_workers', default=10, type=int)
    parser.add_argument('--pin_mem', action='store_true',
                        help='Pin CPU memory in DataLoader for more efficient (sometimes) transfer to GPU.')
                        
    # distributed training parameters
    parser.add_argument('--world_size', default=1, type=int,
                        help='number of distributed processes')
    parser.add_argument('--local_rank', default=-1, type=int)
    parser.add_argument('--dist_on_itp', action='store_true')
    parser.add_argument('--dist_url', default='env://',
                        help='url used to set up distributed training')

    return parser

def build_dataset(data_split, args):
    mean = [0.5]
    std = [0.5]

    transform = transforms.Compose([
        transforms.Grayscale(num_output_channels=1),
        transforms.ToTensor(),
        transforms.Normalize(mean, std),
    ])
    assert data_split in ["train", "test", "valid"]
    root = os.path.join(args.data_path, data_split)
    dataset = datasets.ImageFolder(root, transform=transform)
    print(dataset)
    return dataset

@torch.no_grad()
def evaluate(image, model, device, if_stat=False):
    criterion = torch.nn.CrossEntropyLoss()

    metric_logger = misc.MetricLogger(delimiter="  ")
    header = 'Test:'

    # switch to evaluation mode
    model.eval()

    # for batch in metric_logger.log_every(data_loader, 10, header):
    images = torch.from_numpy(image.reshape(1, 1, 40, 40)).half()
    print(images.shape)
    images = images.to(device, non_blocking=True)

    # compute output
    with torch.cuda.amp.autocast():
        output = model(images)
    
    print(output)

    _, pred = output.topk(1, 1, True, True)
    pred = pred.t()

    print(_, pred)

def load_model(args):
    misc.init_distributed_mode(args)

    print('job dir: {}'.format(os.path.dirname(os.path.realpath(__file__))))
    print("{}".format(args).replace(', ', ',\n'))

    device = torch.device(args.device)

    # fix the seed for reproducibility
    seed = args.seed + misc.get_rank()
    torch.manual_seed(seed)
    np.random.seed(seed)

    cudnn.benchmark = True

    model = models_net_mamba.__dict__[args.model](
        num_classes=args.nb_classes,
        drop_path_rate=args.drop_path,
    )

    trainable_params, all_param = count_parameters(model)
    print("trainable params: {:d} || all params: {:d} || trainable%: {:.4f}".format(
        trainable_params, all_param, 100 * trainable_params / all_param
    ))

    checkpoint = torch.load(args.checkpoint, map_location='cpu')
    print("Load pre-trained checkpoint from: %s" % args.checkpoint)
    checkpoint_model = checkpoint['model']
    state_dict = model.state_dict()
    for k in ['head.weight', 'head.bias']:
        if k in checkpoint_model and checkpoint_model[k].shape != state_dict[k].shape:
            print(f"Removing key {k} from checkpoint")
            del checkpoint_model[k]
    
    #interpolate position embedding
    interpolate_pos_embed(model, checkpoint_model)

    #load checkpoint
    msg = model.load_state_dict(checkpoint_model, strict=False)
    print(msg)

    # manually initialize fc layer
    trunc_normal_(model.head.weight, std=2e-5)

    model.to(device)
    model_without_ddp = model
    n_parameters = sum(p.numel() for p in model.parameters() if p.requires_grad)

    print("Model = %s" % str(model_without_ddp))
    print('number of params (M): %.2f' % (n_parameters / 1.e6))

    eff_batch_size = args.batch_size * args.accum_iter * misc.get_world_size()

    if args.lr is None:  # only base_lr is specified
        args.lr = args.blr * eff_batch_size / 256

    print("base lr: %.2e" % (args.lr * 256 / eff_batch_size))
    print("actual lr: %.2e" % args.lr)

    print("accumulate grad iterations: %d" % args.accum_iter)
    print("effective batch size: %d" % eff_batch_size)

    if args.distributed:
        model = torch.nn.parallel.DistributedDataParallel(model, device_ids=[args.gpu])
        model_without_ddp = model.module

    # build optimizer with layer-wise lr decay (lrd)
    param_groups = lrd.param_groups_lrd(model_without_ddp, args.weight_decay,
                                        no_weight_decay_list=model_without_ddp.no_weight_decay(),
                                        layer_decay=args.layer_decay
                                        )
    optimizer = torch.optim.AdamW(param_groups, lr=args.lr)
    # loss_scaler = NativeScaler()
    # amp about
    amp_autocast = suppress
    loss_scaler = "none"

    misc.load_model(args=args, model_without_ddp=model_without_ddp, optimizer=optimizer, loss_scaler=loss_scaler)

    return model, device


def packet_callback(packet, packet_num, model, device):
    name = f"{packet[IP].src}:{packet.sport}_{packet[IP].dst}:{packet.dport}_{packet.proto}"
    wrpcap(f"{name}.pcap", packet, append=True)
    packet_num.setdefault(name, 0)
    packet_num[name] += 1
    if packet_num[name] == 5:
        image_filename = f"{name}.png"
        stat_filename = image_filename.replace(".png", ".json")
        res = read_5hp_list(f"{name}.pcap")[0]
        flow_array = res.pop("data")
        image = Image.fromarray(flow_array.reshape(40, 40).astype(np.uint8))
        image.save(image_filename)
        with open(stat_filename, "w") as f:
            json.dump(res, f)
        res = evaluate(flow_array, model, device)

if __name__ == '__main__':
    args = get_args_parser()
    args = args.parse_args()
    model, device = load_model(args)
    packet_num = {}
    sniff(prn=lambda x: packet_callback(x, packet_num, model, device), count=20, store=1)