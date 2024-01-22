#include "carpet_bombing_extractor_generated_filter.h"

#include <map>

#include "utils.h"

template<typename T>
CarpetBombingExtractorGeneratedFilter<T>::CarpetBombingExtractorGeneratedFilter(std::map<int64_t, uint32_t> payload_hash_count, std::map<T, uint32_t> flow_label, AbstractFlowIdentification<T>* flow_identification){
    this->payload_hash_count_ = payload_hash_count;
    this->flow_label_ = flow_label;
    this->flow_identification_ = flow_identification;
}

template<typename T>
bool CarpetBombingExtractorGeneratedFilter<T>::accept(PktInfo pkt_info){
    if (payload_hash_count_.find(pkt_info.payload_hash) != payload_hash_count_.end()){
        if (payload_hash_count_[pkt_info.payload_hash] > 1){
            if (flow_label_[flow_identification_->get_flow_id(pkt_info.flow_id)] == MALICIOUS || flow_label_[flow_identification_->get_flow_id(pkt_info.flow_id)] == MALICIOUS_AND_BENIGN){
                return true;
            }
        }
    }
    return false;
}

template<typename T>
void CarpetBombingExtractorGeneratedFilter<T>::reset(){
    // do nothing
}

template class CarpetBombingExtractorGeneratedFilter<uint16_t>;
template class CarpetBombingExtractorGeneratedFilter<uint32_t>;
template class CarpetBombingExtractorGeneratedFilter<FiveTuple>;