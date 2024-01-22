#include "utils.h"

#include <string.h>

#include <iostream>
#include <iomanip>

#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

const PktInfo pkt_info_template::syn_tcp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(false, true, false, true, false, false, false), timeval());
const PktInfo pkt_info_template::udp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(true, false, false, false, false, false, false), timeval());
const PktInfo pkt_info_template::icmp = PktInfo(FiveTuple(0, 0, 0, 0, 0), 0, PktType(false, false, true, false, false, false, false), timeval());

timeval timeval_minus(const timeval &a, const timeval &b){
    timeval ans;
    if (a.tv_usec < b.tv_usec){
        ans.tv_sec = a.tv_sec - b.tv_sec - 1;
        ans.tv_usec = a.tv_usec + 1000000000 - b.tv_usec;
    }
    else{
        ans.tv_sec = a.tv_sec - b.tv_sec;
        ans.tv_usec = a.tv_usec - b.tv_usec;
    }
    return ans;
}

timeval timeval_plus(const timeval &a, const timeval &b){
    timeval ans;
    ans.tv_sec = a.tv_sec + b.tv_sec;
    ans.tv_usec = a.tv_usec + b.tv_usec;
    if (ans.tv_usec >= 1000000000){
        ans.tv_sec += 1;
        ans.tv_usec -= 1000000000;
    }
    return ans;
}

bool timeval_less(const timeval &a, const timeval &b){
    if (a.tv_sec < b.tv_sec){
        return true;
    }
    else if (a.tv_sec == b.tv_sec){
        if (a.tv_usec < b.tv_usec){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

PktInfo raw_pkt_to_pkt_info(pcap_pkthdr *pkt_header, const u_char *pkt_content, ExtraPktInfo *extra_pkt_info){
    PktInfo pkt_info;
    pkt_info.pkt_len = 0;
    const struct ether_header *ethernet_header;
    const struct ip *ip_header;
    const struct tcphdr *tcp_header;
    const struct udphdr *udp_header;
    ethernet_header = (struct ether_header*)pkt_content;
    if (ntohs(ethernet_header->ether_type)==ETHERTYPE_IP) {
        ip_header = (struct ip*)(pkt_content + sizeof(struct ether_header));
        pkt_info.flow_id.dst_ip = ip_header->ip_dst.s_addr;
        pkt_info.flow_id.src_ip = ip_header->ip_src.s_addr;
        pkt_info.flow_id.proto = ip_header->ip_p;

        pkt_info.pkt_len = ntohs(ip_header->ip_len);
        pkt_info.pkt_time = pkt_header->ts;

        extra_pkt_info->ip_fragment_status = IP_FRAGMENT_STATUS_NOT_FRAGMENT;
        if ((ip_header->ip_off & IP_MF) && ((ntohs(ip_header->ip_off) & IP_OFFMASK) == 0)){
            extra_pkt_info->ip_fragment_status = IP_FRAGMENT_STATUS_FIRST_FRAGMENT;
            extra_pkt_info->ip_fragment_id = ntohs(ip_header->ip_id);
        }
        else if ((ntohs(ip_header->ip_off) & IP_OFFMASK) != 0){
            extra_pkt_info->ip_fragment_status = IP_FRAGMENT_STATUS_FRAGMENT;
            extra_pkt_info->ip_fragment_id = ntohs(ip_header->ip_id);
        }

        if ((ntohs(ip_header->ip_off) & IP_OFFMASK) != 0){
            return pkt_info;
        }
        if (ip_header->ip_p == IPPROTO_UDP){
            udp_header = (struct udphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
            pkt_info.flow_id.src_port = ntohs(udp_header->uh_sport);
            pkt_info.flow_id.dst_port = ntohs(udp_header->uh_dport);
            
            pkt_info.pkt_type.udp = true;

            extra_pkt_info->payload_start = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct udphdr);
        }
        if (ip_header->ip_p == IPPROTO_TCP){
            tcp_header = (struct tcphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
            pkt_info.flow_id.src_port = ntohs(tcp_header->th_sport);
            pkt_info.flow_id.dst_port = ntohs(tcp_header->th_dport);

            pkt_info.pkt_type.tcp = true;

            if(tcp_header->syn){pkt_info.pkt_type.tcp_syn = true;}
            if(tcp_header->fin){pkt_info.pkt_type.tcp_fin = true;}
            if(tcp_header->rst){pkt_info.pkt_type.tcp_rst = true;}
            if(tcp_header->ack){pkt_info.pkt_type.tcp_ack = true;}

            extra_pkt_info->payload_start = sizeof(struct ether_header) + sizeof(struct ip) + tcp_header->th_off * 4;
        }
        if (ip_header->ip_p == IPPROTO_ICMP){
            pkt_info.pkt_type.icmp = true;

            extra_pkt_info->payload_start = sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct icmphdr);
        }
    }

    return pkt_info;
}

void pkt_info_to_raw_pkt(PktInfo pkt_info, u_char *pkt_content_template, uint32_t capture_length, pcap_pkthdr *pkt_header, u_char *pkt_content){
    memcpy(pkt_content, pkt_content_template, capture_length);
    struct ip *ip_header = (struct ip*)(pkt_content + sizeof(struct ether_header));
    ip_header->ip_src.s_addr = pkt_info.flow_id.src_ip;
    ip_header->ip_dst.s_addr = pkt_info.flow_id.dst_ip;
    ip_header->ip_len = htons(pkt_info.pkt_len);
    if(pkt_info.pkt_type.tcp){
        struct tcphdr *tcp_header = (struct tcphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
        tcp_header->th_sport = htons(pkt_info.flow_id.src_port);
        tcp_header->th_dport = htons(pkt_info.flow_id.dst_port);

        if(pkt_info.pkt_type.tcp_syn){
            tcp_header->th_flags |= TH_SYN;
        }
        if(pkt_info.pkt_type.tcp_fin){
            tcp_header->th_flags |= TH_FIN;
        }
        if(pkt_info.pkt_type.tcp_rst){
            tcp_header->th_flags |= TH_RST;
        }
        if(pkt_info.pkt_type.tcp_ack){
            tcp_header->th_flags |= TH_ACK;
        }
    }
    else if(pkt_info.pkt_type.udp){
        struct udphdr *udp_header = (struct udphdr*)(pkt_content + sizeof(struct ether_header) + sizeof(struct ip));
        udp_header->uh_sport = htons(pkt_info.flow_id.src_port);
        udp_header->uh_dport = htons(pkt_info.flow_id.dst_port);
    }

    pkt_header->ts.tv_sec = pkt_info.pkt_time.tv_sec;
    pkt_header->ts.tv_usec = pkt_info.pkt_time.tv_usec;
    uint32_t frame_length = (uint32_t)pkt_info.pkt_len + (uint32_t)sizeof(struct ether_header);
    pkt_header->caplen = std::min(capture_length, frame_length);
    pkt_header->len = frame_length;
}

uint32_t ip_str_to_uint(char* str_ip){
    uint32_t uint_ip;
    inet_pton(AF_INET, str_ip, &uint_ip);
    return uint_ip;
}

char str_ip_mem[INET_ADDRSTRLEN];
char* ip_uint_to_str(uint32_t uint_ip){
    inet_ntop(AF_INET, &uint_ip, str_ip_mem, INET_ADDRSTRLEN);
    return str_ip_mem;
}

uint32_t prefix_len_to_mask(uint32_t prefix_len){
    uint32_t mask = 0;
    for(int i = 0; i < prefix_len; i++){
        mask = mask << 1;
        mask += 1;
    }
    mask = mask << (32 - prefix_len);
    return htonl(mask);
}

bool prefix_match(uint32_t ip, uint32_t prefix, uint32_t mask){
    return (ip & mask) == prefix;
}

void print_original_packet(const u_char *pkt_content, pcap_pkthdr *pkt_header){
    std::cout << std::dec << pkt_header->ts.tv_sec << std::endl;
    std::cout << pkt_header->ts.tv_usec << std::endl;
    std::cout << pkt_header->caplen << std::endl;
    std::cout << pkt_header->len<< std::endl;
    for(int i = 0; i < pkt_header->caplen; i++){
        std::cout << std::setw(2) << std::setfill('0') << std::hex << int(pkt_content[i]) << " ";
    }
    std::cout << std::endl;
}

bool FiveTuple::operator<(const FiveTuple &a)const{
    if(src_ip < a.src_ip){
        return true;
    }
    else if(src_ip == a.src_ip){
        if(dst_ip < a.dst_ip){
            return true;
        }
        else if(dst_ip == a.dst_ip){
            if(src_port < a.src_port){
                return true;
            }
            else if(src_port == a.src_port){
                if(dst_port < a.dst_port){
                    return true;
                }
                else if(dst_port == a.dst_port){
                    if(proto < a.proto){
                        return true;
                    }
                    else{
                        return false;
                    }
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

bool FiveTuple::operator==(const FiveTuple &a)const{
    return (src_ip == a.src_ip) && (dst_ip == a.dst_ip) && (src_port == a.src_port) && (dst_port == a.dst_port) && (proto == a.proto);
}

bool FiveTuple::operator!=(const FiveTuple &a)const{
    return !(*this == a);
}

std::ostream& operator<<(std::ostream& os, const FiveTuple& five_tuple){
    char src_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(five_tuple.src_ip), src_ip_str, INET_ADDRSTRLEN);
    char dst_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(five_tuple.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    os << "{";
    os << "\"sip\":";
    os << "\"" << src_ip_str << "\"";
    os << ",";
    os << "\"dip\":";
    os << "\"" << dst_ip_str << "\"";
    os << ",";
    os << "\"sport\":";
    os << five_tuple.src_port;
    os << ",";
    os << "\"dport\":";
    os << five_tuple.dst_port;
    os << ",";
    os << "\"proto\":";
    os << (uint32_t)five_tuple.proto;
    os << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const PktInfo& pkt_info){
    os << "{";
    os << "\"ts\":";
    os << pkt_info.pkt_time.tv_sec;
    os << ",";
    os << "\"tns\":";
    os << pkt_info.pkt_time.tv_usec;
    os << ",";
    os << "\"fid\":";
    os << pkt_info.flow_id;
    os << ",";
    os << "\"len\":";
    os << pkt_info.pkt_len;
    os << ",";
    os << "\"type\":";
    os << pkt_info.pkt_type;
    os << ",";
    os << "\"hash\":";
    os << pkt_info.pkt_hash;
    os << ",";
    os << "\"phash\":";
    os << pkt_info.payload_hash;
    os << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const PktType& pkt_type){
    bool first = true;
    os << "[";
    if(pkt_type.tcp){
        os << "\"tcp\"";
        first = false;
    }
    if(pkt_type.udp){
        if(!first){
            os << ",";
        }
        os << "\"udp\"";
        first = false;
    }
    if(pkt_type.icmp){
        if(!first){
            os << ",";
        }
        os << "\"icmp\"";
        first = false;
    }
    if(pkt_type.tcp_syn){
        if(!first){
            os << ",";
        }
        os << "\"tcp_syn\"";
        first = false;
    }
    if(pkt_type.tcp_fin){
        if(!first){
            os << ",";
        }
        os << "\"tcp_fin\"";
        first = false;
    }
    if(pkt_type.tcp_rst){
        if(!first){
            os << ",";
        }
        os << "\"tcp_rst\"";
        first = false;
    }
    if(pkt_type.tcp_ack){
        if(!first){
            os << ",";
        }
        os << "\"tcp_ack\"";
        first = false;
    }
    os << "]";
    return os;
}