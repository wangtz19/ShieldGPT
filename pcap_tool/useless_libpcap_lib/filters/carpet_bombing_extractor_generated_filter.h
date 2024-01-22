#ifndef CARPET_BOMBING_EXTRACTOR_GENERATED_FILTER_H
#define CARPET_BOMBING_EXTRACTOR_GENERATED_FILTER_H

#include "abstract_filter.h"

#include <map>

#include "abstract_flow_identification.h"

template<typename T>
class CarpetBombingExtractorGeneratedFilter: public AbstractFilter{
  public:
    CarpetBombingExtractorGeneratedFilter(std::map<int64_t, uint32_t> payload_hash_count, std::map<T, uint32_t> flow_label, AbstractFlowIdentification<T>* flow_identification);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    std::map<int64_t, uint32_t> payload_hash_count_;
    std::map<T, uint32_t> flow_label_;
    AbstractFlowIdentification<T>* flow_identification_;

    uint32_t UNCERTAIN = 0;
    uint32_t MALICIOUS = 1;
    uint32_t BENIGN = 2;
    uint32_t MALICIOUS_AND_BENIGN = 3;
};

#endif