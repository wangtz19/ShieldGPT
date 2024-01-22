#ifndef DUMMY_FEATURE_EXTRACTOR_H
#define DUMMY_FEATURE_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <sstream>

#include "utils.h"
#include "dst_ip_flow_identification.h"

template<typename T>
class DummyFeatureExtractor : public FeatureExtractorTemplate<T>{
  public:
    DummyFeatureExtractor(std::string name = std::string("DummyFeatureExtractor"), FeatureWriter* writer = nullptr, AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification());

  private:
    std::vector<T> get_flow_id_list_() override;
    std::string dump_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    PktInfo current_pkt_info_;
    std::ostringstream output_string_stream_;
};

#endif