#include "random_src_port_strategy.h"

#include <random>
#include <vector>

#include "utils.h"

RandomSrcPortStrategy::RandomSrcPortStrategy(std::vector<uint16_t> src_ports){
    src_ports_ = src_ports;
}

PktInfo RandomSrcPortStrategy::apply(PktInfo pkt_info){
    if(src_ports_.size() == 0){
        pkt_info.flow_id.src_port = (rand() % (0xff + 1)) << 8 | (rand() % (0xff + 1));
    }
    else{
        pkt_info.flow_id.src_port = src_ports_[rand() % src_ports_.size()];
    }
    return pkt_info;
}