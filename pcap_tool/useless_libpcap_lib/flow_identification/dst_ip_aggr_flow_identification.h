#ifndef DST_IP_AGGR_FLOW_IDENTIFICATION_H
#define DST_IP_AGGR_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

class DstIpAggrFlowIdentification : public AbstractFlowIdentification<uint32_t>{
  public:
    DstIpAggrFlowIdentification(std::map<uint32_t, uint32_t> aggregation_configuration);

    uint32_t get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(uint32_t flow_id) override;

  private:
    std::map<uint32_t, uint32_t> aggregation_configuration_;
};

#endif