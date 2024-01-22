#include "src_ip_filter.h"

#include "utils.h"

SrcIpFilter::SrcIpFilter(char* prefix_str, uint32_t prefix_len){
    prefix_ = ip_str_to_uint(prefix_str);
    mask_ = prefix_len_to_mask(prefix_len);
}

SrcIpFilter::SrcIpFilter(uint32_t prefix, uint32_t prefix_len){
    prefix_ = prefix;
    mask_ = prefix_len_to_mask(prefix_len);
}

bool SrcIpFilter::accept(PktInfo pkt_info){
    return prefix_match(pkt_info.flow_id.src_ip, prefix_, mask_);
}

void SrcIpFilter::reset(){
    // Do nothing
}