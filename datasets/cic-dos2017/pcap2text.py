from scapy.all import rdpcap, IP, TCP
import numpy as np
import json
import glob
from tqdm import tqdm
import os

def generate_digest(pcap_file_path):
    packets = rdpcap(pcap_file_path) # assume that the pcap file only contains packets in a single flow
    if len(packets) < 2:
        return
    # Get statistic features for this flow
    total_packets = len(packets)
    bytes_list = [len(packet) for packet in packets]
    total_bytes = sum(bytes_list)
    min_packet_size = min(bytes_list)
    max_packet_size = max(bytes_list)
    avg_packet_size = total_bytes / total_packets
    std_packet_size = np.std(bytes_list)

    packet_intervals = [float(packets[i+1].time) - float(packets[i].time) for i in range(total_packets - 1)]
    min_packet_interval = min(packet_intervals)
    max_packet_interval = max(packet_intervals)
    avg_packet_interval = sum(packet_intervals) / len(packet_intervals)
    std_packet_interval = np.std(packet_intervals)

    fct = float(packets[-1].time) - float(packets[0].time)
    packet_rate = total_packets / fct
    byte_rate = total_bytes / fct

    packet = packets[0]
    src_ip = packet["IP"].src
    dst_ip = packet["IP"].dst
    raw_proto = packet["IP"].proto
    proto = {6: "TCP", 17: "UDP", 1: "ICMP"}.get(raw_proto, raw_proto)
    if proto == "TCP":
        try:
            src_port = packet["TCP"].sport
            dst_port = packet["TCP"].dport
        except:
            src_port = ""
            dst_port = ""
    elif proto == "UDP":
        try:
            src_port = packet["UDP"].sport
            dst_port = packet["UDP"].dport
        except:
            src_port = ""
            dst_port = ""
    else:
        src_port = ""
        dst_port = ""

    # Get key fields for first five packets
    # including: src_ip, src_port, dst_ip, dst_port, protocol, packet_size, timestamp, tcp.flags, tcp.window_size, payload
    key_fields = []
    for packet in packets[:5]:
        if IP in packet:
            packet_size = len(packet)
            timestamp = float(packet.time)
            if proto == "TCP":
                tcp_flags = str(packet["TCP"].flags)
                tcp_window_size = packet["TCP"].window
                try:
                    payload = packet["TCP"].payload.original.decode("utf-8")
                except IndexError:
                    payload = ""
                except:
                    payload = str(packet["TCP"].payload.original)
            elif proto == "UDP":
                tcp_flags = ""
                tcp_window_size = ""
                try:
                    payload = packet["UDP"].payload.original.decode("utf-8")
                except IndexError:
                    payload = ""
                except:
                    payload = str(packet["UDP"].payload.original)
            else:
                tcp_flags = ""
                tcp_window_size = ""
                payload = ""
            key_fields.append({
                "packet_size": packet_size,
                "timestamp": timestamp,
                "tcp_flags": tcp_flags,
                "tcp_window_size": tcp_window_size,
                "payload": payload,
            })

    # with open("flow_features.txt", "w") as f:
    #     f.write(f"包数量：{total_packets} 个\n")
    #     f.write(f"字节数：{total_bytes} 字节\n")
    #     f.write(f"最小包大小：{min_packet_size} 字节\n")
    #     f.write(f"最大包大小：{max_packet_size} 字节\n")
    #     f.write(f"平均包大小：{avg_packet_size} 字节\n")
    #     f.write(f"包大小标准差：{std_packet_size} 字节\n")
    #     f.write(f"最小包间隔：{min_packet_interval} 秒\n")
    #     f.write(f"最大包间隔：{max_packet_interval} 秒\n")
    #     f.write(f"平均包间隔：{avg_packet_interval} 秒\n")
    #     f.write(f"包间隔标准差：{std_packet_interval} 秒\n")
    #     f.write(f"流持续时间：{fct} 秒\n")
    #     f.write(f"包速率：{packet_rate} 个/秒\n")
    #     f.write(f"字节速率：{byte_rate} 字节/秒\n")
    #     f.write(f"源 IP：{src_ip}\n")
    #     f.write(f"源端口：{src_port}\n")
    #     f.write(f"目的 IP：{dst_ip}\n")
    #     f.write(f"目的端口：{dst_port}\n")
    #     f.write(f"协议：{proto}\n")
    #     for idx in range(len(key_fields)):
    #         item = key_fields[idx]
    #         f.write(f"第 {idx + 1} 个包：\n")
    #         f.write(f"  包大小：{item['packet_size']} 字节\n")
    #         f.write(f"  时间戳：{item['timestamp']} 秒\n")
    #         f.write(f"  TCP 标志：{item['tcp_flags']}\n")
    #         f.write(f"  TCP 窗口大小：{item['tcp_window_size']}\n")
    #         f.write(f"  载荷：{item['payload']}\n")

    res = {
        "total_packets": total_packets,
        "total_bytes": total_bytes,
        "min_packet_size": min_packet_size,
        "max_packet_size": max_packet_size,
        "avg_packet_size": avg_packet_size,
        "std_packet_size": std_packet_size,
        "min_packet_interval": min_packet_interval,
        "max_packet_interval": max_packet_interval,
        "avg_packet_interval": avg_packet_interval,
        "std_packet_interval": std_packet_interval,
        "flow_completion_time": fct,
        "packet_rate": packet_rate,
        "byte_rate": byte_rate,
        "src_ip": src_ip,
        "src_port": src_port,
        "dst_ip": dst_ip,
        "dst_port": dst_port,
        "proto": proto,
        "packet_info": key_fields,
    }
    with open(pcap_file_path.replace(".pcap", ".json"), "w") as f:
        json.dump(res, f, indent=2)

if __name__ == "__main__":
    pcap_filenames = glob.glob("flow_pcap_5_tuple/*/*/*.pcap")
    for pcap_filename in tqdm(pcap_filenames):
        if os.path.exists(pcap_filename.replace(".pcap", ".json")):
            continue
        generate_digest(pcap_filename)