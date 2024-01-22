#include "random_dst_port_strategy.h"

#include <random>
#include <vector>

#include "utils.h"

RandomDstPortStrategy::RandomDstPortStrategy(std::vector<uint16_t> dst_ports){
    dst_ports_ = dst_ports;
}

PktInfo RandomDstPortStrategy::apply(PktInfo pkt_info){
    if(dst_ports_.size() == 0){
        pkt_info.flow_id.dst_port = (rand() % (0xff + 1)) << 8 | (rand() % (0xff + 1));
    }
    else{
        pkt_info.flow_id.dst_port = dst_ports_[rand() % dst_ports_.size()];
    }
    return pkt_info;
}