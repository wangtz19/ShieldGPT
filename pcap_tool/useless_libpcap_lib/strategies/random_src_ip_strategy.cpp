#include "random_src_ip_strategy.h"

#include <random>
#include <vector>

#include "utils.h"

RandomSrcIpStrategy::RandomSrcIpStrategy(std::vector<uint32_t> src_ips){
    src_ips_ = src_ips;
}

PktInfo RandomSrcIpStrategy::apply(PktInfo pkt_info){
    if(src_ips_.size() == 0){
        pkt_info.flow_id.src_ip = (rand() % (0xff + 1)) << 24 | (rand() % (0xff + 1)) << 16 | (rand() % (0xff + 1)) << 8 | (rand() % (0xff + 1));
    }
    else{
        pkt_info.flow_id.src_ip = src_ips_[rand() % src_ips_.size()];
    }
    return pkt_info;
}