#include "dummy_flow_identification.h"

uint16_t DummyFlowIdentification::get_flow_id(FiveTuple five_tuple){
    return 0;
}

std::string DummyFlowIdentification::dump_flow_id(uint16_t flow_id){
    return "\"naf\"";
}