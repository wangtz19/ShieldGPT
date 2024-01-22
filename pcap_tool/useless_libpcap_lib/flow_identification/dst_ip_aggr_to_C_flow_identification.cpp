#include "dst_ip_aggr_to_C_flow_identification.h"

DstIpAggrToCFlowIdentification::DstIpAggrToCFlowIdentification() {

}

uint32_t DstIpAggrToCFlowIdentification::get_flow_id(FiveTuple five_tuple) {
    return five_tuple.dst_ip & 0x00FFFFFF;
}

std::string DstIpAggrToCFlowIdentification::dump_flow_id(uint32_t flow_id) {
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(flow_id), ip_str, INET_ADDRSTRLEN);
    std::string ret;
    ret += "\"";
    ret += ip_str;
    ret += "\"";
    return ret;
}