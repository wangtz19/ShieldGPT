#include "flowlens_feature_extractor.h"

#include <iostream>

#include "utils.h"

template<typename T>
FlowlensFeatureExtractor<T>::FlowlensFeatureExtractor(size_t quant_len, size_t size_bin_cnt, size_t dtime_bin_cnt, size_t seq_len, std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification) : FeatureExtractorTemplate<T>(name, writer, filter, flow_identification){
    quant_len_ = quant_len;
    size_bin_cnt_ = size_bin_cnt;
    dtime_bin_cnt_ = dtime_bin_cnt;
    seq_len_ = seq_len;
}

template<typename T>
uint32_t FlowlensFeatureExtractor<T>::get_inter_pkt_time_(T flow_id, timeval new_pkt_time){
    uint32_t inter_pkt_time = 0;
    uint32_t int_new_pkt_time = new_pkt_time.tv_sec * 1000000000 + new_pkt_time.tv_usec;
    auto iter = last_pkt_time_.find(flow_id);
    if(iter != last_pkt_time_.end()){
        inter_pkt_time = int_new_pkt_time - iter->second;
    }
    last_pkt_time_[flow_id] = int_new_pkt_time;
    return inter_pkt_time / 1000000;
}

template<typename T>
uint32_t FlowlensFeatureExtractor<T>::get_quant_(uint32_t raw_feature){
    return raw_feature >> quant_len_;
}

template<typename T>
bool FlowlensFeatureExtractor<T>::is_ready_(T flow_id){
    return pkt_count_[flow_id] == seq_len_;
}

template<typename T>
std::vector<T> FlowlensFeatureExtractor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    for(auto iter = pkt_dtime_features_.begin(); iter != pkt_dtime_features_.end(); iter++){
        if (is_ready_(iter->first))
            flow_id_list.push_back(iter->first);
    }
    return flow_id_list;
}

template<typename T>
void FlowlensFeatureExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    uint32_t inter_packet_time = get_inter_pkt_time_(flow_id, pkt_info.pkt_time);
    if (pkt_dtime_features_.find(flow_id) == pkt_dtime_features_.end()){
        pkt_dtime_features_[flow_id] = std::vector<uint32_t>(dtime_bin_cnt_, 0);
        pkt_size_features_[flow_id] = std::vector<uint32_t>(size_bin_cnt_, 0);
        pkt_count_[flow_id] = 0;
    }
    if (get_quant_(pkt_info.pkt_len) > size_bin_cnt_){
        std::cout << "pkt_len > size_bin_cnt_ ";
        std::cout << pkt_info.pkt_len << std::endl;
    }
    if (get_quant_(inter_packet_time) > dtime_bin_cnt_){
        std::cout << "inter_packet_time > dtime_bin_cnt_ ";
        std::cout << inter_packet_time << std::endl;
    }
    pkt_size_features_[flow_id][std::min((size_t)get_quant_(pkt_info.pkt_len), size_bin_cnt_)] += 1;
    pkt_dtime_features_[flow_id][std::min((size_t)get_quant_(inter_packet_time), dtime_bin_cnt_)] += 1;
    pkt_count_[flow_id] += 1;
    if(is_ready_(flow_id)){
        this->print_flow_feature(pkt_info.flow_id);
        pkt_dtime_features_.erase(flow_id);
        pkt_size_features_.erase(flow_id);
        pkt_count_.erase(flow_id);
        this->update_packet_counter(flow_id, seq_len_);
    }
}

template<typename T>
std::string FlowlensFeatureExtractor<T>::dump_flow_feature_(T flow_id){
    std::string ret_str;
    ret_str += "{";
    ret_str += "\"pkt_size\":";
    ret_str += "[";
    for(int i = 0; i < seq_len_ - 1;i ++){ret_str += std::to_string(pkt_size_features_[flow_id][i]) + ",";}
    ret_str += std::to_string(pkt_size_features_[flow_id][seq_len_ - 1]);
    ret_str += "]";
    ret_str += ",";
    ret_str += "\"pkt_dtime\":";
    ret_str += "[";
    for(int i = 0; i < seq_len_ - 1;i ++){ret_str += std::to_string(pkt_dtime_features_[flow_id][i]) + ",";}
    ret_str += std::to_string(pkt_dtime_features_[flow_id][seq_len_ - 1]);
    ret_str += "]";
    ret_str += "}";
    return ret_str;
}

template<typename T>
void FlowlensFeatureExtractor<T>::reset_(){
    pkt_size_features_.clear();
    pkt_dtime_features_.clear();
    last_pkt_time_.clear();
}

template class FlowlensFeatureExtractor<uint16_t>;
template class FlowlensFeatureExtractor<uint32_t>;
template class FlowlensFeatureExtractor<FiveTuple>;