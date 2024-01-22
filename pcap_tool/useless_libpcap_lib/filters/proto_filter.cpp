#include "proto_filter.h"

#include "utils.h"

ProtoFilter::ProtoFilter(uint8_t proto){
    proto_ = proto;
}

bool ProtoFilter::accept(PktInfo pkt_info){
    return (pkt_info.flow_id.proto == proto_);
}

void ProtoFilter::reset(){
    // Do nothing
}