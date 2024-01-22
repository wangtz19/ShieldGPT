#ifndef DST_IP_AGGR_TO_C_FLOW_IDENTIFICATION_H
#define DST_IP_AGGR_TO_C_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

class DstIpAggrToCFlowIdentification : public AbstractFlowIdentification<uint32_t>{
  public:
    DstIpAggrToCFlowIdentification();

    uint32_t get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(uint32_t flow_id) override;
};

#endif