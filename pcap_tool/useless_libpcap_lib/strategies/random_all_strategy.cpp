#include "random_all_strategy.h"

#include "utils.h"
#include "random_src_port_strategy.h"
#include "random_src_ip_strategy.h"
#include "random_dst_port_strategy.h"
#include "random_dst_ip_strategy.h"
#include "random_pkt_size_strategy.h"
#include "random_inter_packet_time_strategy.h"

RandomAllStrategy::RandomAllStrategy(){
    strategies_.push_back(new RandomSrcPortStrategy());
    strategies_.push_back(new RandomSrcIpStrategy());
    strategies_.push_back(new RandomDstPortStrategy());
    strategies_.push_back(new RandomDstIpStrategy());
    strategies_.push_back(new RandomPktSizeStrategy());
    strategies_.push_back(new RandomInterPacketTimeStrategy());
}

PktInfo RandomAllStrategy::apply(PktInfo pkt_info){
    for(auto strategy: strategies_){
        pkt_info = strategy->apply(pkt_info);
    }
    return pkt_info;
}