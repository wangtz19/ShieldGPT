#ifndef FEATURE_EXTRACTOR_TEMPLATE_H
#define FEATURE_EXTRACTOR_TEMPLATE_H

#include "abstract_feature_extractor.h"

#include <string>
#include <vector>

#include "utils.h"
#include "feature_writer.h"
#include "abstract_filter.h"
#include "abstract_flow_identification.h"

template<typename T>
class FeatureExtractorTemplate: public AbstractFeatureExtractor{
  public:
    FeatureExtractorTemplate(std::string name, FeatureWriter* writer, AbstractFilter* filter, AbstractFlowIdentification<T>* flow_identification);

    void append_packet(PktInfo pkt_info) override;
    void print_feature() override;
    void reset() override;
    void print_packet_counter();
  
  protected:
    void print_flow_feature(FiveTuple five_tuple);
    void update_packet_counter(T flow_id, uint32_t pkt_num);
  
  private:
    virtual std::vector<T> get_flow_id_list_() = 0;
    virtual std::string dump_flow_feature_(T flow_id) = 0;
    virtual void append_packet_info_(T flow_id, PktInfo pkt_info) = 0;
    virtual void reset_() = 0;
    
    std::string name_;
    FeatureWriter* writer_;
    AbstractFilter* filter_;
    AbstractFlowIdentification<T>* flow_identification_;
    std::map<T, uint32_t> pkt_count_;
    std::map<T, uint32_t> pkt_acked_;
};

#endif