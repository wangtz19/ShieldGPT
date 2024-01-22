/* 
  * igonre non-ipv4 packets
  * ip_len is set 1500 if ip_len > 1500 (for PktInfo but not for orginial packet content)
*/

#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "abstract_traffic_generator.h"

#include <map>

#include <pcap.h>

#include "utils.h"

class PcapReader: public AbstractTrafficGenerator {
  public:
    PcapReader(const char *pcap_file_dir, bool enable_payload_hash_check = false, bool enable_pkt_hash_check = false, bool enable_original_pkt = false, bool enable_ip_fragment_check = false);

    void get_curr_original_pkt(u_char *pkt_content, pcap_pkthdr *pkt_header);

    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;

  private:
    int64_t hash_func(const u_char *pkt_content, uint32_t start, uint32_t end);

    pcap_t *pcap_descr_;
    timeval start_time_;
    PktInfo curr_pkt_info_;
    std::map<uint16_t, PktInfo> ip_fragment_reconstruct_;
    bool is_end_;

    bool enable_payload_hash_check_;
    bool enable_pkt_hash_check_;
    bool enable_original_pkt_;
    bool enable_ip_fragment_check_;
    u_char curr_pkt_content_[10000];
    pcap_pkthdr curr_pkt_header_;
};

#endif