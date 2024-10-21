#include "five_tuple_flow_identification.h"

FiveTuple FiveTupleFlowIdentification::get_flow_id(FiveTuple five_tuple){
    return five_tuple;
}

std::string FiveTupleFlowIdentification::dump_flow_id(FiveTuple flow_id, std::string flow_prefix){
    // char src_ip_str[INET_ADDRSTRLEN];
    char src_ip_str[INET6_ADDRSTRLEN];
    char dst_ip_str[INET6_ADDRSTRLEN];
    if (flow_id.ipv6) {
        inet_ntop(AF_INET6, flow_id.src_ipv6, src_ip_str, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, flow_id.dst_ipv6, dst_ip_str, INET6_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET, &(flow_id.src_ip), src_ip_str, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(flow_id.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    }
    // char dst_ip_str[INET_ADDRSTRLEN];
    std::string ret = flow_prefix;
    ret += src_ip_str;
    ret += "_";
    ret += std::to_string(flow_id.src_port);
    ret += "_";
    ret += std::to_string((uint32_t)flow_id.proto);
    ret += "_";
    ret += dst_ip_str;
    ret += "_";
    ret += std::to_string(flow_id.dst_port);
    return ret;
}