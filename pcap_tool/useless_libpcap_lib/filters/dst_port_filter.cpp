#include "dst_port_filter.h"

#include "utils.h"

DstPortFilter::DstPortFilter(uint16_t port){
    min_port_ = port;
    max_port_ = port;
}

DstPortFilter::DstPortFilter(uint16_t min_port, uint16_t max_port){
    min_port_ = min_port;
    max_port_ = max_port;
}

bool DstPortFilter::accept(PktInfo pkt_info){
    return ((pkt_info.flow_id.dst_port >= min_port_) && (pkt_info.flow_id.dst_port <= max_port_));
}

void DstPortFilter::reset(){
    // do nothing
}