#include "src_port_filter.h"

#include "utils.h"

SrcPortFilter::SrcPortFilter(uint16_t port){
    min_port_ = port;
    max_port_ = port;
}

SrcPortFilter::SrcPortFilter(uint16_t min_port, uint16_t max_port){
    min_port_ = min_port;
    max_port_ = max_port;
}

bool SrcPortFilter::accept(PktInfo pkt_info){
    return ((pkt_info.flow_id.src_port >= min_port_) && (pkt_info.flow_id.src_port <= max_port_));
}

void SrcPortFilter::reset(){
    // Do nothing
}