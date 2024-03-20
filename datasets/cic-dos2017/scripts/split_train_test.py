import os
import random
from tqdm import tqdm

# set random seed
random.seed(0)

# train_ratio, test_ratio, val_ratio = 0.8, 0.1, 0.1
TRAIN_NUM = 5
TEST_NUM = 10 - TRAIN_NUM
train_ratio, test_ratio = TRAIN_NUM / 10, TEST_NUM / 10

attack_types = ["benign", "goldeneye", "hulk", "rudy", 
                "slowbody2", "slowheaders", "slowloris", "slowread"]
for attack_type in attack_types:
    filenames = os.listdir(f"../image/attack/{attack_type}")
    random.shuffle(filenames)
    train_num = int(len(filenames) * train_ratio)
    test_num = len(filenames) - train_num
    train_filenames = filenames[:train_num]
    test_filenames = filenames[train_num:]
    os.makedirs(f"../image/{TRAIN_NUM}-{TEST_NUM}/train/{attack_type}", exist_ok=True)
    os.makedirs(f"../image/{TRAIN_NUM}-{TEST_NUM}/test/{attack_type}", exist_ok=True)
    pbar = tqdm(total=len(filenames))
    for filename in train_filenames:
        # os.rename(f"flow_pcap_5_tuple/{attack_type}/{filename}",
        #           f"flow_pcap_5_tuple/train/{attack_type}/{filename}")
        os.system(f"cp ../image/attack/{attack_type}/{filename} ../image/{TRAIN_NUM}-{TEST_NUM}/train/{attack_type}/{filename}")
        pbar.update(1)
    for filename in test_filenames:
        # os.rename(f"flow_pcap_5_tuple/{attack_type}/{filename}",
        #           f"flow_pcap_5_tuple/test/{attack_type}/{filename}")
        os.system(f"cp ../image/attack/{attack_type}/{filename} ../image/{TRAIN_NUM}-{TEST_NUM}/test/{attack_type}/{filename}")
        pbar.update(1)