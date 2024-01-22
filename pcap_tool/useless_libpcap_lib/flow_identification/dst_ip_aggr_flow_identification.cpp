#include "dst_ip_aggr_flow_identification.h"

#include <iostream>

DstIpAggrFlowIdentification::DstIpAggrFlowIdentification(std::map<uint32_t, uint32_t> aggregation_configuration){
    aggregation_configuration_ = aggregation_configuration;
}

uint32_t DstIpAggrFlowIdentification::get_flow_id(FiveTuple five_tuple){
    auto iter = aggregation_configuration_.find(five_tuple.dst_ip);
    if(iter != aggregation_configuration_.end()){
        return iter->second;
    }
    else {
        return 0;
    }
}

std::string DstIpAggrFlowIdentification::dump_flow_id(uint32_t flow_id){
    std::string ret;
    ret += "\"";
    ret += std::to_string(flow_id);
    ret += "\"";
    return ret;
}