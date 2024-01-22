#include "traffic_generator.h"

#include "utils.h"
#include "abstract_strategy.h"

TrafficGenerator::TrafficGenerator(PktInfo template_pkt_info, timeval start_time, timeval stop_time, std::vector<AbstractStrategy*> strategies) {
    template_pkt_info_ = template_pkt_info;
    curr_pkt_info_ = template_pkt_info;
    curr_pkt_info_.pkt_time = start_time;
    stop_time_ = stop_time;
    strategies_ = strategies;
}

void TrafficGenerator::add_strategy(AbstractStrategy* strategy) {
    strategies_.push_back(strategy);
}

PktInfo TrafficGenerator::get_current_pkt_info() {
    return curr_pkt_info_;
}

timeval TrafficGenerator::generate_next() {
    timeval prev_time = curr_pkt_info_.pkt_time;
    curr_pkt_info_ = template_pkt_info_;
    curr_pkt_info_.pkt_time = prev_time;
    for (auto strategy: strategies_) {
        curr_pkt_info_ = strategy->apply(curr_pkt_info_);
    }
    return curr_pkt_info_.pkt_time;
}

bool TrafficGenerator::is_end() {
    if (timeval_less(stop_time_, curr_pkt_info_.pkt_time)) {
        return true;
    }
    return false;
}

void TrafficGenerator::close() {
    
}