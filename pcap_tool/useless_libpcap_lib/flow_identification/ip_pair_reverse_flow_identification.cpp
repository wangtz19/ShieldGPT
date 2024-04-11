#include "ip_pair_reverse_flow_identification.h"

#include "utils.h"

FiveTuple IpPairReverseFlowIdentification::get_flow_id(FiveTuple five_tuple){
    FiveTuple new_five_tuple = five_tuple;
    new_five_tuple.src_port = 0;
    new_five_tuple.dst_port = 0;
    new_five_tuple.proto = 0;
    return new_five_tuple;
}

std::string IpPairReverseFlowIdentification::dump_flow_id(FiveTuple flow_id, std::string flow_prefix){
    char src_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.src_ip), src_ip_str, INET_ADDRSTRLEN);
    char dst_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    std::string ret = flow_prefix;
    ret += dst_ip_str;
    ret += "_";
    ret += src_ip_str;
    return ret; // dst_ip_src_ip
}