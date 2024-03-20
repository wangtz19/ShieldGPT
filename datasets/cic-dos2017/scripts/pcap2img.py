import os
import glob
import binascii
from PIL import Image
import scapy.all as scapy
from tqdm import tqdm
import numpy as np

PAD_IP_ADDR = "0.0.0.0"

def read_5hp_list(pcap_dir):
    packets = scapy.rdpcap(pcap_dir)
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


image_dirname = "flow_image"
pcap_dirname = "flow_pcap_5_tuple"
pcap_filenames = glob.glob(f"{pcap_dirname}/*/*/*.pcap")
for pcap_filename in tqdm(pcap_filenames):
    content = read_5hp_list(pcap_filename)
    content = np.array([int(content[i:i + 2], 16) for i in range(0, len(content), 2)])
    fh = np.reshape(content, (40, 40))
    fh = np.uint8(fh)
    im = Image.fromarray(fh)
    image_filename = pcap_filename.replace('.pcap','.png').replace(pcap_dirname, image_dirname)
    os.makedirs(os.path.dirname(image_filename), exist_ok=True)
    im.save(image_filename)