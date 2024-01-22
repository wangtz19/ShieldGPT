#include "dummy_feature_extractor.h"

template<typename T>
DummyFeatureExtractor<T>::DummyFeatureExtractor(std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification): FeatureExtractorTemplate<T>(name, writer, filter, flow_identification){
    reset_();
}

template<typename T>
std::vector<T> DummyFeatureExtractor<T>::get_flow_id_list_(){
    std::vector<T> flow_id_list;
    return flow_id_list;
}

template<typename T>
std::string DummyFeatureExtractor<T>::dump_flow_feature_(T flow_id){
    output_string_stream_.str("");
    output_string_stream_ << current_pkt_info_;
    std::string ret = output_string_stream_.str();
    return ret;
}

template<typename T>
void DummyFeatureExtractor<T>::append_packet_info_(T flow_id, PktInfo pkt_info){
    current_pkt_info_ = pkt_info;
    this->print_flow_feature(pkt_info.flow_id);
}

template<typename T>
void DummyFeatureExtractor<T>::reset_(){
    current_pkt_info_ = PktInfo();
    output_string_stream_.str("");
}

template class DummyFeatureExtractor<uint16_t>;
template class DummyFeatureExtractor<uint32_t>;
template class DummyFeatureExtractor<FiveTuple>;