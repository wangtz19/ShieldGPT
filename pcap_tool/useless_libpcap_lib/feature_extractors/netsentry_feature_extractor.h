#ifndef NETSENTRY_FEATURE_EXTRACTOR_H
#define NETSENTRY_FEATURE_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <stddef.h>

#include "utils.h"
#include "dst_ip_flow_identification.h"

template<typename T>
class NetsentryFeatureExtractor :public FeatureExtractorTemplate<T>{
  public:
    NetsentryFeatureExtractor(size_t dft_sequence_length = 32, size_t dft_feature_length = 3, size_t seq_len = 1500, std::string name = std::string("NetsentryFeatureExtractor"), FeatureWriter* writer = nullptr, AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification());

  private:
    uint32_t get_inter_pkt_time_(T flow_id, timeval new_pkt_time);
    uint32_t pkt_feature_embed_(uint32_t pkt_len, PktType pkt_type, uint32_t inter_pkt_time);
    bool is_ready_(T flow_id);
    
    std::vector<T> get_flow_id_list_() override;
    std::string dump_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    MapVec<T, uint32_t> pkt_features_;
    std::map<T, uint32_t> last_pkt_time_;
    size_t dft_sequence_length_;
    size_t dft_feature_length_;
    size_t seq_len_;
};

#endif