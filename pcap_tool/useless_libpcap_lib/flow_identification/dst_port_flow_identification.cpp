#include "dst_port_flow_identification.h"

uint16_t DstPortFlowIdendification::get_flow_id(FiveTuple five_tuple){
    return five_tuple.dst_port;
}

std::string DstPortFlowIdendification::dump_flow_id(uint16_t flow_id){
    std::string ret;
    ret += "\"";
    ret += std::to_string(flow_id);
    ret += "\"";
    return ret;
}