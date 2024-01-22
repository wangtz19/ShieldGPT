import os
import random

# set random seed
random.seed(0)

# train_ratio, test_ratio, val_ratio = 0.8, 0.1, 0.1
train_ratio, test_ratio = 0.8, 0.2

attack_types = ["benign", "goldeneye", "hulk", "rudy", 
                "slowbody2", "slowheaders", "slowloris", "slowread"]
for attack_type in attack_types:
    filenames = os.listdir(f"flow_pcap_5_tuple/{attack_type}")
    random.shuffle(filenames)
    train_num = int(len(filenames) * train_ratio)
    # test_num = int(len(filenames) * test_ratio)
    # val_num = len(filenames) - train_num - test_num
    test_num = len(filenames) - train_num
    train_filenames = filenames[:train_num]
    # test_filenames = filenames[train_num:train_num+test_num]
    # val_filenames = filenames[train_num+test_num:]
    test_filenames = filenames[train_num:]
    os.makedirs(f"flow_pcap_5_tuple/train/{attack_type}", exist_ok=True)
    os.makedirs(f"flow_pcap_5_tuple/test/{attack_type}", exist_ok=True)
    # os.makedirs(f"flow_pcap_5_tuple/val/{attack_type}", exist_ok=True)
    for filename in train_filenames:
        os.rename(f"flow_pcap_5_tuple/{attack_type}/{filename}",
                  f"flow_pcap_5_tuple/train/{attack_type}/{filename}")
    for filename in test_filenames:
        os.rename(f"flow_pcap_5_tuple/{attack_type}/{filename}",
                  f"flow_pcap_5_tuple/test/{attack_type}/{filename}")
    # for filename in val_filenames:
    #     os.rename(f"flow_pcap_5_tuple/{attack_type}/{filename}",
    #               f"flow_pcap_5_tuple/val/{attack_type}/{filename}")
    os.removedirs(f"flow_pcap_5_tuple/{attack_type}")