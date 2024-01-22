#ifndef SRC_IP_FLOW_IDENTIFICATION_H
#define SRC_IP_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

class SrcIpFlowIdentification : public AbstractFlowIdentification<uint32_t>{
  public:
    uint32_t get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(uint32_t flow_id) override;
};

#endif