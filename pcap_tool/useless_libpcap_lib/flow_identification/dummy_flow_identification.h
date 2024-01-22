#ifndef DUMMY_FLOW_IDENTIFICATION_H
#define DUMMY_FLOW_IDENTIFICATION_H

#include "abstract_flow_identification.h"

class DummyFlowIdentification : public AbstractFlowIdentification<uint16_t>{
  public:
    uint16_t get_flow_id(FiveTuple five_tuple) override;
    std::string dump_flow_id(uint16_t flow_id) override;
};

#endif