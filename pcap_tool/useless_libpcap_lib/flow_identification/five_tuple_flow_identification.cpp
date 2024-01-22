#include "five_tuple_flow_identification.h"

FiveTuple FiveTupleFlowIdentification::get_flow_id(FiveTuple five_tuple){
    return five_tuple;
}

std::string FiveTupleFlowIdentification::dump_flow_id(FiveTuple flow_id){
    char src_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.src_ip), src_ip_str, INET_ADDRSTRLEN);
    char dst_ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id.dst_ip), dst_ip_str, INET_ADDRSTRLEN);
    std::string ret;
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