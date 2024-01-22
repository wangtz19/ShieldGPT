import subprocess
from datetime import datetime, timedelta
import json
import os
import shutil
from scapy.all import rdpcap
from tqdm import tqdm

pcap_file = "AppDDos.pcap"
start_time = datetime.fromisoformat("2010-06-12 03:01:06")
end_time = datetime.fromisoformat("2010-06-13 03:01:05")

with open("attacks_dict.json", "r") as f:
    attacks_dict = json.load(f)

victim_ip_set = set()
for attack, val_list in attacks_dict.items():
    for val in val_list:
        victim_ip_set.add(val["dst_ip"])

os.makedirs("flow_pcap/benign", exist_ok=True)
filenames = list(filter(lambda x: x.endswith(".pcap"), 
                        os.listdir("flow_pcap")))
for filename in tqdm(filenames):
    dst_ip = filename.split("_")[0] # filename format: dstip_srcip.pcap
    if dst_ip not in victim_ip_set:
        # copy to benign folder
        shutil.copy(f"flow_pcap/{filename}", f"flow_pcap/benign/{filename}")
    else:
        # get time span for current pcap file
        packets = rdpcap(f"flow_pcap/{filename}")
        cur_start_time = packets[0].time
        cur_end_time = packets[-1].time
        for attack, val_list in attacks_dict.items():
            for val in val_list:
                if val["dst_ip"] == dst_ip:
                    target_start_time = start_time + timedelta(minutes=val["start_offset"])
                    if val["end_offset"] is not None:
                        target_end_time = start_time + timedelta(minutes=val["end_offset"])
                    else:
                        target_end_time = end_time
                    if cur_start_time <= target_start_time.timestamp() and \
                        cur_end_time >= target_end_time.timestamp():
                        # cut pcap file
                        os.makedirs(f"flow_pcap/{attack}", exist_ok=True)
                        same_name_files = list(filter(lambda x: x.startswith(filename[:-len(".pcap")]),
                                                      os.listdir(f"flow_pcap/{attack}")))
                        rank = len(same_name_files)
                        subprocess.run(["editcap", "-A", target_start_time.strftime("%Y-%m-%d %H:%M:%S"),
                                        "-B", target_end_time.strftime("%Y-%m-%d %H:%M:%S"),
                                        f"flow_pcap/{filename}",
                                        f"flow_pcap/{attack}/{filename[:-len('.pcap')]}-{rank}.pcap"])
        
