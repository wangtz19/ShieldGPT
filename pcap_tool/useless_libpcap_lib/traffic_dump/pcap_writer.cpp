#include "pcap_writer.h"

#include <string.h>

#include <iostream>

#include <pcap/dlt.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
    
PcapWriter::PcapWriter(const char *pcap_file_dir, uint32_t kCaplen){
    kCaplen_ = kCaplen;
    generate_template();
    pcap_descr_ = pcap_open_dead_with_tstamp_precision(DLT_EN10MB, kCaplen_, PCAP_TSTAMP_PRECISION_NANO);
    pcap_dumper_ = pcap_dump_open(pcap_descr_, pcap_file_dir);
    if(pcap_dumper_ == NULL){
        std::cout << "pcap_dumper == NULL" << std::endl;
    }
}

void PcapWriter::dump_original_pkt(const u_char *pkt_content, pcap_pkthdr *pkt_header){
    pcap_dump((u_char *)pcap_dumper_, pkt_header, pkt_content);
}

void PcapWriter::dump_pkt_info(PktInfo pkt_info){
    pcap_pkthdr *pkt_header = new pcap_pkthdr();
    u_char *pkt_content = new u_char[kCaplen_];
    pkt_info_to_raw_pkt(pkt_info, tcp_template_, kCaplen_, pkt_header, pkt_content);
    pcap_dump((u_char *)pcap_dumper_, pkt_header, pkt_content);
}

void PcapWriter::close_dump_file(){
    pcap_dump_close(pcap_dumper_);
}

void PcapWriter::generate_template(){
    const ether_addr *kSrcMac = ether_aton("90:e2:ba:8a:22:65"); // DC6
    const ether_addr *kDstMac = ether_aton("90:e2:ba:8a:c7:a0"); // I5
    
    // TCP template
    // Ethernet header
    struct ether_header *ethernet_header = (struct ether_header*)tcp_template_;
    memcpy(ethernet_header->ether_shost, kSrcMac, ETHER_ADDR_LEN);
    memcpy(ethernet_header->ether_dhost, kDstMac, ETHER_ADDR_LEN);
    ethernet_header->ether_type = ntohs((uint16_t)ETHERTYPE_IP);
    // IP header
    struct ip *ip_header = (struct ip*)(tcp_template_ + sizeof(struct ether_header));
    ip_header->ip_v = 4;
    ip_header->ip_hl = 5;
    ip_header->ip_tos = 0;
    ip_header->ip_len = htons(40);
    ip_header->ip_id = 0;
    ip_header->ip_off = 0;
    ip_header->ip_ttl = 64;
    ip_header->ip_p = IPPROTO_TCP;
    ip_header->ip_sum = 0;
    ip_header->ip_src.s_addr = inet_addr("0.0.0.0");
    ip_header->ip_dst.s_addr = inet_addr("0.0.0.0");
    // TCP header
    struct tcphdr *tcp_header = (struct tcphdr*)(tcp_template_ + sizeof(struct ether_header) + sizeof(struct ip));
    tcp_header->th_sport = htons(0);
    tcp_header->th_dport = htons(0);
    tcp_header->th_seq = htonl(0);
    tcp_header->th_ack = htonl(0);
    tcp_header->th_off = 5;
    tcp_header->th_flags = 0;
    tcp_header->th_win = htons(65535);
    tcp_header->th_sum = 0;
    tcp_header->th_urp = 0;
}