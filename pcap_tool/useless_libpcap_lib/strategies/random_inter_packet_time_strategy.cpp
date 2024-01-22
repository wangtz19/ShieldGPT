#include "random_inter_packet_time_strategy.h"

#include <random>
#include <vector>

#include "utils.h"

RandomInterPacketTimeStrategy::RandomInterPacketTimeStrategy(std::vector<timeval> inter_packet_times){
    inter_packet_times_ = inter_packet_times;
}

PktInfo RandomInterPacketTimeStrategy::apply(PktInfo pkt_info){
    if(inter_packet_times_.size() == 0){
        timeval random_inter_packet_time = timeval();
        random_inter_packet_time.tv_sec = 0;
        random_inter_packet_time.tv_usec = rand() % 1000;
        pkt_info.pkt_time = timeval_plus(pkt_info.pkt_time, random_inter_packet_time);
    }
    else{
        pkt_info.pkt_time = timeval_plus(pkt_info.pkt_time, inter_packet_times_[rand() % inter_packet_times_.size()]);
    }
    return pkt_info;
}