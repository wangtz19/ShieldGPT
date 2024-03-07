import os
import pandas as pd
from tqdm import tqdm
import binascii
from PIL import Image
import numpy as np
import scapy.all as scapy

PAD_IP_ADDR = "0.0.0.0"

def read_5hp_list(pcap_filename):
    try:
        packets = scapy.rdpcap(pcap_filename)
    except:
        print(f"Error: {pcap_filename} cannot be read")
        return None
    data = []
    for packet in packets[:5]:
        try:
            ip = packet['IP']
        except:
            continue
        ip.src, ip.dst = PAD_IP_ADDR, PAD_IP_ADDR
        header = (binascii.hexlify(bytes(ip))).decode()
        try:
            payload = (binascii.hexlify(bytes(packet['Raw']))).decode()
            header = header.replace(payload, '')
        except:
            payload = ''
        if len(header) > 160:
            header = header[:160]
        elif len(header) < 160:
            header += '0' * (160 - len(header))
        if len(payload) > 480:
            payload = payload[:480]
        elif len(payload) < 480:
            payload += '0' * (480 - len(payload))
        data.append((header, payload))
    if len(data) < 5:
        for i in range(5-len(data)):
            data.append(('0'*160, '0'*480))
    final_data = ''
    for h, p in data:
        final_data += h
        final_data += p
    return final_data

# attacks = ["PortMap", "LDAP", "MSSQL", "NetBIOS", "SYN", "UDP-Lag", "UDP"]
attacks = ["MSSQL"]
attack_map = {
    "LDAP": "LDAP",
    "MSSQL": "MSSQL",
    "NetBIOS": "NetBIOS",
    "Portmap": "PortMap",
    "Syn": "SYN",
    "UDPLag": "UDP-Lag",
    "UDP": "UDP",
    "BENIGN": "benign"
}
reverse_map = {v: k for k, v in attack_map.items()}

os.makedirs(f"../pcap/flow_image/benign", exist_ok=True)
for attack in attacks:
    print(f"Checking {attack}")
    df = pd.read_csv(f"../csv/03-11/{reverse_map[attack]}.csv")
    name2labels = {}
    for idx in tqdm(range(len(df))):
        row = df.iloc[idx]
        name = str(row[" Source IP"]) + "_" + str(row[" Source Port"]) + "_" + str(row[" Protocol"]) \
                + "_" + str(row[" Destination IP"]) + "_" + str(row[" Destination Port"])
        if name not in name2labels:
            name2labels[name] = row[" Label"]
        elif name2labels[name] != row[" Label"]:
            # print(f"Error: [{attack}] {name} has different labels")
            name2labels[name] = "BOTH" # discard flows with inconsistent labels
    os.makedirs(f"../pcap/flow_image/{attack}", exist_ok=True)
    pcap_filenames = os.listdir(f"../pcap/flow_pcap_5_tuple/{attack}")
    for pcap_filename in tqdm(pcap_filenames):
        pcap_flow_name = pcap_filename.replace(".pcap", "")
        if pcap_flow_name not in name2labels or name2labels[pcap_flow_name] == "BOTH":
            continue
        content = read_5hp_list(f"../pcap/flow_pcap_5_tuple/{attack}/{pcap_filename}")
        if content is None:
            continue
        content = np.array([int(content[i:i + 2], 16) for i in range(0, len(content), 2)])
        fh = np.reshape(content, (40, 40))
        fh = np.uint8(fh)
        im = Image.fromarray(fh)
        label = attack_map[name2labels[pcap_flow_name]]
        image_filename = f"../pcap/flow_image/{label}/{pcap_filename.replace('.pcap','.png')}"
        os.makedirs(os.path.dirname(image_filename), exist_ok=True)
        im.save(image_filename)
    