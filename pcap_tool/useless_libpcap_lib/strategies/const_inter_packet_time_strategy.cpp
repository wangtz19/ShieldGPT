#include "const_inter_packet_time_strategy.h"

#include "utils.h"

PktInfo ConstInterPacketTimeStrategy::apply(PktInfo pkt_info){
    pkt_info.pkt_time = timeval_plus(pkt_info.pkt_time, inter_packet_time_);
    return pkt_info;
}

ConstInterPacketTimeStrategy::ConstInterPacketTimeStrategy(timeval inter_packet_time){
    inter_packet_time_ = inter_packet_time;
}