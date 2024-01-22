/* 
  * igonre non-ipv4 packets
  * ip_len is set 1500 if ip_len > 1500 (for PktInfo but not for orginial packet content)
*/

#include "pcap_reader.h"

#include <string.h>
#include <iostream>

#include <pcap.h>

#include "utils.h"

PcapReader::PcapReader(const char *pcap_file_dir, bool enable_payload_hash_check, bool enable_pkt_hash_check, bool enable_original_pkt, bool enable_ip_fragment_check){
    enable_payload_hash_check_ = enable_payload_hash_check;
    enable_pkt_hash_check_ = enable_pkt_hash_check;
    enable_original_pkt_ = enable_original_pkt;
    enable_ip_fragment_check_ = enable_ip_fragment_check;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_descr_ = pcap_open_offline_with_tstamp_precision(pcap_file_dir, PCAP_TSTAMP_PRECISION_NANO, errbuf);
    if (pcap_descr_ == NULL) {
        std::cout << "pcap_open_offline()failed:" << errbuf << std::endl;
    }
    start_time_ = timeval();
    start_time_ = generate_next();
    curr_pkt_info_.pkt_time = timeval_minus(curr_pkt_info_.pkt_time, start_time_);
    ip_fragment_reconstruct_.clear();
    is_end_ = false;
}

void PcapReader::get_curr_original_pkt(u_char *pkt_content, pcap_pkthdr *pkt_header){
    memcpy(pkt_content, curr_pkt_content_, curr_pkt_header_.caplen);
    memcpy(pkt_header, &curr_pkt_header_, sizeof(pcap_pkthdr));
}

PktInfo PcapReader::get_current_pkt_info(){
    return curr_pkt_info_;
}

timeval PcapReader::generate_next(){
    while(true){
        const u_char *pkt_content;
        pcap_pkthdr *pkt_header = new pcap_pkthdr();
        pkt_content = pcap_next(pcap_descr_, pkt_header);
        if(pkt_content == NULL){
            is_end_ = true;
            return pkt_header->ts;
        }
        ExtraPktInfo extra_pkt_info;
        curr_pkt_info_ = raw_pkt_to_pkt_info(pkt_header, pkt_content, &extra_pkt_info);
        if(curr_pkt_info_.pkt_len == 0){ // ignore non ipv4 packets
            continue;
        }
        if(curr_pkt_info_.pkt_len > 1500){
            curr_pkt_info_.pkt_len = 1500;
        }
        if(enable_ip_fragment_check_){
            if(extra_pkt_info.ip_fragment_status == 1){
                if(ip_fragment_reconstruct_.find(extra_pkt_info.ip_fragment_id) == ip_fragment_reconstruct_.end()){
                    ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id] = curr_pkt_info_;
                }
                else{
                    std::cout << "Error from pcap reader: ip fragment id collision: " << "(" << ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id] << ")" << " " << "(" << curr_pkt_info_ << ")" << std::endl;
                }
            }
            else if(extra_pkt_info.ip_fragment_status == 2){
                if(ip_fragment_reconstruct_.find(extra_pkt_info.ip_fragment_id) == ip_fragment_reconstruct_.end()){
                    std::cout << "Error from pcap reader: ip fragment id not found: " << "(" << curr_pkt_info_ << ")" << std::endl;
                }
                else if (curr_pkt_info_.flow_id.src_ip != ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id].flow_id.src_ip || curr_pkt_info_.flow_id.dst_ip != ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id].flow_id.dst_ip){
                    std::cout << "Error from pcap reader: ip fragment with same id but different src ip or dst ip: " << "(" << ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id] << ")" << " " << "(" << curr_pkt_info_ << ")" << std::endl;
                }
                else{
                    curr_pkt_info_.flow_id.dst_port = ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id].flow_id.dst_port;
                    curr_pkt_info_.flow_id.src_port = ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id].flow_id.src_port;
                    curr_pkt_info_.pkt_type = ip_fragment_reconstruct_[extra_pkt_info.ip_fragment_id].pkt_type;
                }
            }
        }
        if(enable_original_pkt_){
            memcpy(curr_pkt_content_, pkt_content, pkt_header->caplen);
            memcpy(&curr_pkt_header_, pkt_header, sizeof(pcap_pkthdr));
        }
        if(enable_pkt_hash_check_){
            curr_pkt_info_.pkt_hash = hash_func(pkt_content, 0, std::min(pkt_header->caplen, curr_pkt_info_.pkt_len + 14));
        }
        if(enable_payload_hash_check_){
            curr_pkt_info_.payload_hash = hash_func(pkt_content, extra_pkt_info.payload_start, std::min(pkt_header->caplen, curr_pkt_info_.pkt_len + 14));
        }
        curr_pkt_info_.pkt_time = timeval_minus(curr_pkt_info_.pkt_time, start_time_);
        return curr_pkt_info_.pkt_time;
    }
}

bool PcapReader::is_end(){
    return is_end_;
}

void PcapReader::close(){
    pcap_close(pcap_descr_);
}

int64_t PcapReader::hash_func(const u_char *pkt_content, uint32_t start, uint32_t end){
    int64_t hash = 0;
    if(start >= end)
        return -1;
    int64_t hash_mod = 1001145147ll;
    int64_t hash_base = 114ll;
    for(uint32_t i = start; i < end; i += 4){
        int64_t combine_4B = 0;
        for(uint32_t j = 0; j < 4; j++){
            combine_4B = (combine_4B << 8) + ((i + j < end) ? pkt_content[i + j] : 0);
        }
        hash = hash * hash_base + combine_4B;
        hash = hash % hash_mod;
    }
    return hash;
}