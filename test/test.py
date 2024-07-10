from scapy.all import *
from dataset_common import read_5hp_list

import argparse
import json
import numpy as np
import pandas as pd
import os
from pathlib import Path
import torch
import torch.backends.cudnn as cudnn
from torchvision import transforms
from tqdm import tqdm

import util.lr_decay as lrd
import util.misc as misc
from util.misc import NativeScalerWithGradNormCount as NativeScaler
from util.misc import count_parameters

from PIL import Image

from pcap2text import generate_digest
import models_net_mamba
import llm

global packet_num

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

@torch.no_grad()
def evaluate(image, model, device, if_stat=False):
    mean = [0.5]
    std = [0.5]

    transform = transforms.Compose([
        transforms.Grayscale(num_output_channels=1),
        transforms.ToTensor(),
        transforms.Normalize(mean, std),
    ])

    image = transform(image)
    # print(image.shape)
    # print(image)

    # switch to evaluation mode
    model.eval()

    # for batch in metric_logger.log_every(data_loader, 10, header):
    images = image.reshape(1, 1, 40, 40).half()
    # print(images)
    images = images.to(device, non_blocking=True)

    # compute output
    with torch.cuda.amp.autocast():
        output = model(images)
    
    print(output)

    _, pred = output.topk(1, 1, True, True)
    pred = pred.t()

    return pred

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

    checkpoint = torch.load(args.checkpoint, map_location='cpu')
    print("Load checkpoint from: %s" % args.checkpoint)
    checkpoint_model = checkpoint['model']

    #load checkpoint
    msg = model.load_state_dict(checkpoint_model, strict=False)
    print(msg)

    model.to(device)

    return model

def generate_prompt(name, attack):
    attack_dict = {
        "goldeneye": {
            "description": "GoldenEye attack. It is a type of DoS attack that exhausts avaliable TCP socks of the target server by establishing a legitimate HTTP connection and then requires only a few hundred requests at long-term and regular intervals.",
            "name": "GoldenEye",
        },
        "hulk": {
            "description": "Hulk (HTTP Unbearable Load King) attack. It is a type of DoS attack that aims to overwhelm a targeted web server or application with by sending a large number of HTTP GET requests, rendering it unavailable to legitimate users. " + 
                "And attackers attempts to evade detection by employing simple obfuscation techniques, such as using different user-agent strings and referrers.",
            "name": "Hulk",
        },
        "rudy": {
            "description": "RUDY (R U Dead Yet) attack. It is a type of DoS attack that targets web applications by exploiting the HTTP POST method." +
                "Unlike other DoS attacks that flood the server with a large amount of traffic, RUDY is more subtle and focuses on holding the server's resources for as long as possible by sending partial HTTP POST requests to the server",
            "name": "RUDY",
        },
        "slowbody2": {
            "description": "slow-body attack generated by Slowhttptest tool. It is a type of DoS attack that sends incomplete HTTP POST requests with a large Content-Length value to the server and keeps the connection open for as long as possible, which can exhaust the server's resources.",
            "name": "slow-body",
        },
        "slowheaders": {
            # "description": "slow-headers attack generated by Slowhttptest tool. It is a type of DoS attack that sends HTTP requests with incomplete headers and keeps the server wait for a long time, which can exhaust the server's resources.",
            "description": "slow-headers attack generated by Slowhttptest tool. It is a type of DoS attack and the attacker sends HTTP requests in pieces as slow as possible, so the server keeps the connections open, waiting for the headers to arrive. This can exhaust the server's resources, causing a denial of service.",
            "name": "slow-headers",
        },
        "slowloris": {
            # "description": "slowloris attack. It is a type of DoS attack that aims to keep a web server's resources occupied by sending HTTP requests with incomplete headers and then keep the connection open for as long as possible.",
            "description": "slowloris attack. It is a type of DoS attack and the attacker tries to keep many connections to the target web server open and hold them open as long as possible. It accomplishes this by creating connections to the target server, but sending only a partial request.",
            "name": "slowloris",
        },
        "slowread": {
            "description": "slow-read attack generated by Slowhttptest tool. It is a type of DoS attack that sends HTTP requests and then slowly reads the server's response, which can exhaust the server's resources.",
            "name": "slow-read",
        }
    }

    device_dict = {
        "cisco": {
            "description": "an ordinary Cisco router",
        },
        "snort": {
            "description": "a Snort IPS",
        },
        "iptables": {
            "description": "a Linux Server with iptables"
        },
    }

    DEVICE_NAME = "iptables"
    res_list = []
    with open(f"./pcap/{name}.json", "r") as f:
        text = f.read()
    prompt = f"""{text}
The above are traffic statistical characteristics and the first 5 packets' original information of the {attack_dict[attack]["description"]}. 
Suppose you are at {device_dict[DEVICE_NAME]["description"]}. Please just propose one defense strategy to mitigate this {attack_dict[attack]["name"]} attack. 
Note that you don't need to describe your strategy in words, you just need to provide detailed configuration codes based on the above traffic characteristics for the strategy. 
"""
    res_list.append({
        "prompt": prompt,
        "label": attack,
    })

    df = pd.DataFrame(res_list)
    # df.to_csv(f"./pcap/{name}.csv", index=False)
    return df
    

def packet_callback(packet, stream_num, model, model_llm, tokenizer, device):
    # os.fork()
    # if pid != 0:
    #     return
    # print(packet.show())
    global packet_num
    try:
        name = f"{packet[IP].src}:{packet.sport}_{packet[IP].dst}:{packet.dport}_{packet.proto}"
    except:
        # packet_num += 1
        # print(f"packet_num={packet_num}")
        return
    # if not ((packet[IP].src == '172.18.0.12' and packet.sport == 22 and packet[IP].dst == '192.168.1.204') or (packet[IP].dst == '172.18.0.12' and packet.dport == 22 and packet[IP].src == '192.168.1.204')):
    #     packet_num += 1
    #     print(f"packet_num={packet_num}")
    wrpcap(f"./pcap/{name}.pcap", packet, append=True)
    stream_num.setdefault(name, 0)
    stream_num[name] += 1
    if stream_num[name] == 5:
        image_filename = f"./pcap/{name}.png"
        stat_filename = image_filename.replace(".png", ".json")
        res = read_5hp_list(f"./pcap/{name}.pcap")[0]
        flow_array = res.pop("data")
        image = Image.fromarray(flow_array.reshape(40, 40).astype(np.uint8))
        image.save(image_filename)
        generate_digest(f"./pcap/{name}.pcap")
        res = ["benign", "goldeneye", "hulk", "rudy", "slowbody2", "slowheaders", "slowloris", "slowread"][evaluate(image, model, device).item()]
        print(name, res)
        # res = "goldeneye"
        # if res != "benign":
        #     df = generate_prompt(name, res)
        #     tqdm.pandas()
        #     df[f"reply-{'gpt-4'}"] = df["prompt"].progress_apply(
        #         lambda x: llm.llm_generate(x, 'gpt-4', model_llm, tokenizer))
        #     df.to_csv(f"./pcap/{name}.csv", index=False)

if __name__ == '__main__':
    packet_num = 0
    args = get_args_parser()
    args = args.parse_args()
    model = load_model(args)
    model_llm, tokenizer = llm.load_model('gpt-4')
    device = args.device
    stream_num = {}
    os.makedirs('./pcap', exist_ok=True)
    conf.bufsize=104857600
    sniff(prn=lambda x: packet_callback(x, stream_num, model, model_llm, tokenizer, device), store=1)