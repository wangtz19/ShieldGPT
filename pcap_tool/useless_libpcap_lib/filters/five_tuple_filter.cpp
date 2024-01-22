#include "five_tuple_filter.h"

#include "utils.h"
#include "src_ip_filter.h"
#include "dst_ip_filter.h"
#include "src_port_filter.h"
#include "dst_port_filter.h"
#include "proto_filter.h"

FiveTupleFilter::FiveTupleFilter(char* src_ip, char* dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t proto){
    filter_.add_filter(new SrcIpFilter(src_ip, 32));
    filter_.add_filter(new DstIpFilter(dst_ip, 32));
    filter_.add_filter(new SrcPortFilter(src_port));
    filter_.add_filter(new DstPortFilter(dst_port));
    filter_.add_filter(new ProtoFilter(proto));
}

bool FiveTupleFilter::accept(PktInfo pkt_info){
    return filter_.accept(pkt_info);
}

void FiveTupleFilter::reset(){
    filter_.reset();
}