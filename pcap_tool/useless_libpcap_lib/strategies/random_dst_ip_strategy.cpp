#include "random_dst_ip_strategy.h"

#include <random>
#include <vector>

#include "utils.h"

RandomDstIpStrategy::RandomDstIpStrategy(std::vector<uint32_t> dst_ips){
    dst_ips_ = dst_ips;
}

PktInfo RandomDstIpStrategy::apply(PktInfo pkt_info){
    if(dst_ips_.size() == 0){
        pkt_info.flow_id.dst_ip = (rand() % (0xff + 1)) << 24 | (rand() % (0xff + 1)) << 16 | (rand() % (0xff + 1)) << 8 | (rand() % (0xff + 1));
    }
    else{
        pkt_info.flow_id.dst_ip = dst_ips_[rand() % dst_ips_.size()];
    }
    return pkt_info;
}