#include "not_filter.h"

NotFilter::NotFilter(AbstractFilter* filter){
    filter_ = filter;
}

bool NotFilter::accept(PktInfo pkt_info){
    return !filter_->accept(pkt_info);
}

void NotFilter::reset(){
    filter_->reset();
}