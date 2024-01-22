#ifndef FIVE_TUPLE_FLOW_IDENTIFICATION_H
#define FIVE_TUPLE_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

#include "utils.h"

class FiveTupleFlowIdentification : public AbstractFlowIdentification<FiveTuple>{
  public:
    FiveTuple get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(FiveTuple flow_id) override;
};

#endif