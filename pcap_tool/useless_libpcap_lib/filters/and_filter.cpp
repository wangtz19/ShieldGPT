#include "and_filter.h"

AndFilter::AndFilter(){

}

AndFilter::AndFilter(std::vector<AbstractFilter*> filters){
    filters_ = filters;
}

void AndFilter::add_filter(AbstractFilter* filter){
    filters_.push_back(filter);
}

bool AndFilter::accept(PktInfo pkt_info){
    for (auto filter: filters_){
        if (!filter->accept(pkt_info)){
            return false;
        }
    }
    return true;
}

void AndFilter::reset(){
    for (auto filter: filters_){
        filter->reset();
    }
}