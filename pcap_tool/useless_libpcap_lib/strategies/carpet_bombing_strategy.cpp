#include "carpet_bombing_strategy.h"

#include "utils.h"

PktInfo CarpetBombingStrategy::apply(PktInfo pkt_info){
    if(polling_strategy_ == POLLING_STRATEGY_RANDOM){
        pkt_info.flow_id.dst_ip = target_ip_list_[rand() % target_ip_list_.size()];
    }
    if(polling_strategy_ == POLLING_STRATEGY_ROUND_ROBIN){
        pkt_info.flow_id.dst_ip = target_ip_list_[pkt_counter_];
    }
    pkt_counter_ = (pkt_counter_ + 1) % target_ip_list_.size();
    return pkt_info;
}

CarpetBombingStrategy::CarpetBombingStrategy(std::vector<uint32_t> target_ip_list, uint32_t polling_strategy){
    target_ip_list_ = target_ip_list;
    polling_strategy_ = polling_strategy;
}