#ifndef IP_PAIR_REVERSE_FLOW_IDENTIFICATION_H
#define IP_PAIR_REVERSE_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

#include "utils.h"

class IpPairReverseFlowIdentification : public AbstractFlowIdentification<FiveTuple>{
  public:
    FiveTuple get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(FiveTuple flow_id) override;
};

#endif