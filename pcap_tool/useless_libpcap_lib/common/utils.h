#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <ostream>
#include <map>
#include <vector>

#include <pcap.h>

struct PktType {
    bool udp = false;
    bool tcp = false;
    bool icmp = false;
    bool tcp_syn = false;
    bool tcp_fin = false;
    bool tcp_rst = false;
    bool tcp_ack = false;

    PktType(){}
    PktType(bool udp, bool tcp, bool icmp, bool tcp_syn, bool tcp_fin, bool tcp_rst, bool tcp_ack)
        : udp(udp), 
          tcp(tcp), 
          icmp(icmp), 
          tcp_syn(tcp_syn), 
          tcp_fin(tcp_fin), 
          tcp_rst(tcp_rst), 
          tcp_ack(tcp_ack){
    }

    friend std::ostream& operator<<(std::ostream& os, const PktType& pkt_type);
};

struct FiveTuple {
    uint32_t src_ip = 0;
    uint32_t dst_ip = 0;
    uint16_t src_port = 0;
    uint16_t dst_port = 0;
    uint8_t proto = 0;

    FiveTuple(){}
    FiveTuple(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t proto)
        : src_ip(src_ip), 
          dst_ip(dst_ip), 
          src_port(src_port), 
          dst_port(dst_port), 
          proto(proto){
    }

    friend std::ostream& operator<<(std::ostream& os, const FiveTuple& five_tuple);
    bool operator<(const FiveTuple& a) const;
    bool operator==(const FiveTuple& a) const;
    bool operator!=(const FiveTuple& a) const;
};

struct PktInfo {
    FiveTuple flow_id = FiveTuple();
    uint32_t pkt_len = 0;
    PktType pkt_type = PktType();
    timeval pkt_time = {0, 0};
    int64_t pkt_hash = -1;
    int64_t payload_hash = -1;
    
    PktInfo(){}
    PktInfo(FiveTuple flow_id, uint32_t pkt_len, PktType pkt_type, timeval pkt_time, int64_t pkt_hash = -1, int64_t payload_hash = -1)
        : flow_id(flow_id), 
          pkt_len(pkt_len), 
          pkt_type(pkt_type), 
          pkt_time(pkt_time),
          pkt_hash(pkt_hash),
          payload_hash(payload_hash){
    }

    friend std::ostream& operator<<(std::ostream& os, const PktInfo& pkt_info);
};

#define IP_FRAGMENT_STATUS_NOT_FRAGMENT 0
#define IP_FRAGMENT_STATUS_FIRST_FRAGMENT 1
#define IP_FRAGMENT_STATUS_FRAGMENT 2
struct ExtraPktInfo {
    uint16_t ip_fragment_status;
    uint16_t ip_fragment_id;
    uint32_t payload_start;
};

class pkt_info_template {
  public:
    const static PktInfo syn_tcp;
    const static PktInfo udp;
    const static PktInfo icmp;
};

template<class K, typename V>
class MapVec {
  public:
    std::map<K, std::vector<V>> data_;
    void insert(K key, V value){
        auto iter = data_.find(key);
        if(iter != data_.end()){
            iter->second.push_back(value);
        }
        else {
            std::vector<V> _temp_vector = {value};
            data_[key] = _temp_vector;
        }
    }
    void clear(K key){
        data_.find(key)->second.clear();
    }
    std::vector<V> get(K key){
        return data_.find(key)->second;
    }
};

timeval timeval_minus(const timeval &a, const timeval &b);
timeval timeval_plus(const timeval &a, const timeval &b);
bool timeval_less(const timeval &a, const timeval &b);

PktInfo raw_pkt_to_pkt_info(pcap_pkthdr *pkt_header, const u_char *pkt_content, ExtraPktInfo *extra_pkt_info);
void pkt_info_to_raw_pkt(PktInfo pkt_info, u_char *pkt_content_template, uint32_t capture_length, pcap_pkthdr *pkt_header, u_char *pkt_content);

uint32_t ip_str_to_uint(char* str_ip);
char* ip_uint_to_str(uint32_t uint_ip);
uint32_t prefix_len_to_mask(uint32_t prefix_len);
bool prefix_match(uint32_t ip, uint32_t prefix, uint32_t mask);

void print_original_packet(const u_char *pkt_content, pcap_pkthdr *pkt_header);

#endif