#include "dst_ip_filter.h"

#include "utils.h"

DstIpFilter::DstIpFilter(char* prefix_str, uint32_t prefix_len){
    prefix_ = ip_str_to_uint(prefix_str);
    mask_ = prefix_len_to_mask(prefix_len);
}

DstIpFilter::DstIpFilter(uint32_t prefix, uint32_t prefix_len){
    prefix_ = prefix;
    mask_ = prefix_len_to_mask(prefix_len);
}

bool DstIpFilter::accept(PktInfo pkt_info){
    return prefix_match(pkt_info.flow_id.dst_ip, prefix_, mask_);
}

void DstIpFilter::reset(){
    // do nothing
}