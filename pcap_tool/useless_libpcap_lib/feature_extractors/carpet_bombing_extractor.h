#ifndef CARPET_BOMBING_EXTRACTOR_H
#define CARPET_BOMBING_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <string>

#include "utils.h"
#include "carpet_bombing_extractor_generated_filter.h"
#include "dst_port_flow_identification.h"

template<typename T>
class CarpetBombingExtractor: public FeatureExtractorTemplate<T>{
  public:
    CarpetBombingExtractor(std::string name = std::string("CarpetBombingExtractor"), FeatureWriter* writer = nullptr, AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstPortFlowIdendification());
    
    CarpetBombingExtractorGeneratedFilter<T> get_filter();

  private:
    std::vector<T> get_flow_id_list_() override;
    std::string dump_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    std::map<int64_t, uint32_t> payload_hash_count_;
    std::map<int64_t, T> payload_hash_flow_id_;
    std::map<T, uint32_t> flow_label_;
    
    AbstractFlowIdentification<T>* flow_identification_; // UGLY

    uint32_t UNCERTAIN = 0;
    uint32_t MALICIOUS = 1;
    uint32_t BENIGN = 2;
    uint32_t MALICIOUS_AND_BENIGN = 3;
};

#endif