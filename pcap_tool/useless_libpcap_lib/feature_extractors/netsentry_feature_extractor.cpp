#include "netsentry_feature_extractor.h"

#include <iostream>
#include <complex>

#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR
#include "simple_fft/fft.h"
#include "simple_fft/fft_settings.h"

#include "utils.h"

template<typename T>
NetsentryFeatureExtractor<T>::NetsentryFeatureExtractor(size_t dft_sequence_length, size_t dft_feature_length, size_t seq_len, std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification) : FeatureExtractorTemplate<T>(name, writer, filter, flow_identification){
    dft_sequence_length_ = dft_sequence_length;
    dft_feature_length_ = dft_feature_length;
    seq_len_ = seq_len;
}

template<typename T>
uint32_t NetsentryFeatureExtractor<T>::get_inter_pkt_time_(T flow_id, timeval new_pkt_time){
    uint32_t inter_pkt_time = 0;
    uint32_t int_new_pkt_time = new_pkt_time.tv_sec * 1000000000 + new_pkt_time.tv_usec;
    auto iter = last_pkt_time_.find(flow_id);
    if(iter != last_pkt_time_.end()){
        inter_pkt_time = int_new_pkt_time - iter->second;
    }
    last_pkt_time_[flow_id] = int_new_pkt_time;
    return inter_pkt_time;
}

template<typename T>
uint32_t NetsentryFeatureExtractor<T>::pkt_feature_embed_(uint32_t pkt_len, PktType pkt_type, uint32_t inter_pkt_time){
    uint32_t cooked_len = (pkt_len >= 1600) ? (31 << 10) : (pkt_len / 50) << 10;
    
    uint32_t cooked_dtime = 0;
    if(inter_pkt_time > 0) {
        cooked_dtime = ((uint32_t)floor(log2((uint64_t)inter_pkt_time)) > 127) ? (127 << 3) : ((uint32_t)floor(log2((uint64_t)inter_pkt_time)) << 3);\
    }

    uint32_t cooked_proto = 3;
    if (pkt_type.udp){cooked_proto = 1;}
    else if (pkt_type.tcp){
        if(pkt_type.tcp_syn){cooked_proto = 0;}
        else if(pkt_type.tcp_fin){cooked_proto = 4;}
        else if(pkt_type.tcp_rst){cooked_proto = 0;}
        else if(pkt_type.tcp_ack){cooked_proto = 7;}
        else {cooked_proto = 7;}
    }
    else {cooked_proto = 3;}

    uint32_t cooked_feature = cooked_len + cooked_dtime + cooked_proto;
    return cooked_feature;
}

template<typename T>
bool NetsentryFeatureExtractor<T>::is_ready_(T flow_id){
    return pkt_features_.get(flow_id).size() == seq_len_;
}

template<typename T>
std::vector<T> NetsentryFeatureExtractor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    for(auto iter = pkt_features_.data_.begin(); iter != pkt_features_.data_.end(); iter++){
        if (is_ready_(iter->first))
            flow_id_list.push_back(iter->first);
    }
    return flow_id_list;
}

template<typename T>
void NetsentryFeatureExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    uint32_t inter_packet_time = get_inter_pkt_time_(flow_id, pkt_info.pkt_time);
    pkt_features_.insert(flow_id, pkt_feature_embed_(pkt_info.pkt_len, pkt_info.pkt_type, inter_packet_time));
    if(is_ready_(flow_id)){
        this->print_flow_feature(pkt_info.flow_id);
        pkt_features_.clear(flow_id);
        this->update_packet_counter(flow_id, seq_len_);
    }
}

template<typename T>
std::string NetsentryFeatureExtractor<T>::dump_flow_feature_(T flow_id){
    auto feature_array = pkt_features_.get(flow_id);
    std::vector<std::complex<double>> netsentry_feature(dft_feature_length_);
    
    auto iterL = feature_array.begin();
    auto iterR = feature_array.begin() + dft_sequence_length_;
    for (int i = 0; i < (seq_len_ / dft_sequence_length_); i++){
        const char * error = NULL;
        bool ret;
        std::vector<std::complex<double>> fft_result(dft_sequence_length_);
        std::vector<uint32_t> sub_feature_array(iterL, iterR);
        ret = simple_fft::FFT(sub_feature_array, fft_result, dft_sequence_length_, error);
        for (int j = 0; j < dft_feature_length_; j++){
            netsentry_feature[j] += fft_result[j];
        }
        iterL += dft_sequence_length_;
        iterR += dft_sequence_length_;
    }
    
    std::string ret_str;
    ret_str += "[";
    for(int i = 0; i < dft_feature_length_ - 1;i ++){ret_str += std::to_string(netsentry_feature[i].real()) + "," + std::to_string(netsentry_feature[i].imag()) + ",";}
    ret_str += std::to_string(netsentry_feature[dft_feature_length_ - 1].real()) + "," + std::to_string(netsentry_feature[dft_feature_length_ - 1].imag());
    ret_str += "]";
    return ret_str;
}

template<typename T>
void NetsentryFeatureExtractor<T>::reset_(){
    pkt_features_.data_.clear();
    last_pkt_time_.clear();
}

template class NetsentryFeatureExtractor<uint16_t>;
template class NetsentryFeatureExtractor<uint32_t>;
template class NetsentryFeatureExtractor<FiveTuple>;