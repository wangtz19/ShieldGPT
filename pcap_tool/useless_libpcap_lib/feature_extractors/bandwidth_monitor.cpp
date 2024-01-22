#include "bandwidth_monitor.h"

#include <iostream>

#include "utils.h"

template<typename T>
BandwidthMonitor<T>::BandwidthMonitor(timeval interval_time, std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification): FeatureExtractorTemplate<T>(name, writer, filter, flow_identification) {
    interval_time_ = interval_time;
    
    last_time_ = {0, 0};
    current_time_ = {0, 0};
}

template<typename T>
bool BandwidthMonitor<T>::is_ready_(){
    return timeval_less(interval_time_, timeval_minus(current_time_, last_time_));
}

template<typename T>
bool BandwidthMonitor<T>::interval_skipped_(){
    return timeval_less(timeval_plus(interval_time_, interval_time_), timeval_minus(current_time_, last_time_));
}

template<typename T>
std::vector<T> BandwidthMonitor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    if(interval_skipped_()){
        return flow_id_list;
    }
    for(auto it = packet_count_.begin(); it != packet_count_.end(); it ++){
        flow_id_list.push_back(it->first);
    }
    return flow_id_list;
}

template<typename T>
std::string BandwidthMonitor<T>::dump_flow_feature_(T flow_id){
    std::string ret;
    ret += "{";
    // packet count
    ret += "\"p\":";
    ret += std::to_string(packet_count_[flow_id]);
    ret += ",";
    // total packet bits
    ret += "\"b\":";
    ret += std::to_string(packet_bytes_[flow_id] * 8);
    ret += "}";
    return ret;
}

template<typename T>
void BandwidthMonitor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    auto packet_count_it = packet_count_.find(flow_id);
    if(packet_count_it == packet_count_.end()){
        packet_count_[flow_id] = 1;
    }
    else{
        packet_count_[flow_id] ++;
    }
    auto packet_bytes_it = packet_bytes_.find(flow_id);
    if(packet_bytes_it == packet_bytes_.end()){
        packet_bytes_[flow_id] = (uint64_t)pkt_info.pkt_len;
    }
    else{
        packet_bytes_[flow_id] += (uint64_t)pkt_info.pkt_len;
    }
    current_time_ = pkt_info.pkt_time;
    
    if(is_ready_()){
        while(interval_skipped_()){
            this->print_feature();
            last_time_ = timeval_plus(last_time_, interval_time_);
        }
        this->print_feature();
        packet_count_.clear();
        packet_bytes_.clear();
        last_time_ = timeval_plus(last_time_, interval_time_);
    }
}

template<typename T>
void BandwidthMonitor<T>::reset_(){
    packet_count_.clear();
    packet_bytes_.clear();
    last_time_ = {0, 0};
    current_time_ = {0, 0};
}

template class BandwidthMonitor<uint16_t>;
template class BandwidthMonitor<uint32_t>;
template class BandwidthMonitor<FiveTuple>;