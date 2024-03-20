import subprocess
import os
from tqdm import tqdm

attack_types = ["benign", "ddossim", "goldeneye", "hulk", "rudy", 
                "slowbody2", "slowheaders", "slowloris", "slowread"]
os.makedirs("flow_pcap_5_tuple", exist_ok=True)
for attack_type in attack_types:
    os.makedirs(f"flow_pcap_5_tuple/{attack_type}", exist_ok=True)
    for filename in tqdm(os.listdir(f"flow_pcap/{attack_type}")):
        subprocess.run([
            "../../pcap_tool/splitter", # executable, split pcap file into 5-tuple
            f"flow_pcap/{attack_type}/{filename}", # input pcap file
            f"flow_pcap_5_tuple/{attack_type}" # output folder
        ])