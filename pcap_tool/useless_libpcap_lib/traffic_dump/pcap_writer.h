#ifndef PCAP_WRITER_H
#define PCAP_WRITER_H

#include <pcap.h>

#include "utils.h"

#define MAX_CAPLEN 65535


class PcapWriter {
  public:
    PcapWriter(const char *pcap_file_dir, uint32_t kCaplen = 500, bool overwrite = true);

    void dump_original_pkt(const u_char *pkt_content, pcap_pkthdr *pkt_header);
    void dump_pkt_info(PktInfo pkt_info);
    void close_dump_file();

    pcap_dumper_t* get_pcap_dumper() { return pcap_dumper_; }

  private:
    void generate_template();

    u_char tcp_template_[MAX_CAPLEN];
    u_char udp_template_[MAX_CAPLEN];
    u_char icmp_template_[MAX_CAPLEN];

    uint32_t kCaplen_;
    pcap_t *pcap_descr_;
    pcap_dumper_t *pcap_dumper_;
};

#endif