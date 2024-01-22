#ifndef FLOWLENS_FEATURE_EXTRACTOR_H
#define FLOWLENS_FEATURE_EXTRACTOR_H

#include "feature_extractor_template.h"

#include <stddef.h>

#include "utils.h"
#include "dst_ip_flow_identification.h"

template<typename T>
class FlowlensFeatureExtractor :public FeatureExtractorTemplate<T>{
  public:
    FlowlensFeatureExtractor(size_t quant_len = 2, size_t size_bin_cnt = 1000, size_t dtime_bin_cnt = 1000, size_t seq_len = 1500, std::string name = std::string("FlowlensFeatureExtractor"), FeatureWriter* writer = nullptr, AbstractFilter* filter = nullptr, AbstractFlowIdentification<T>* flow_identification = new DstIpFlowIdentification());

  private:
    uint32_t get_inter_pkt_time_(T flow_id, timeval new_pkt_time);
    uint32_t get_quant_(uint32_t raw_feature);
    bool is_ready_(T flow_id);
    
    std::vector<T> get_flow_id_list_() override;
    std::string dump_flow_feature_(T flow_id) override;
    void append_packet_info_(T flow_id, PktInfo pkt_info) override;
    void reset_() override;

    std::map<T, std::vector<uint32_t>> pkt_size_features_;
    std::map<T, std::vector<uint32_t>> pkt_dtime_features_;
    std::map<T, uint32_t> last_pkt_time_;
    std::map<T, uint32_t> pkt_count_;
    size_t quant_len_;
    size_t size_bin_cnt_;
    size_t dtime_bin_cnt_;
    size_t seq_len_;
};

#endif