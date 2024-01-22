#include "or_filter.h"

OrFilter::OrFilter(){

}

OrFilter::OrFilter(std::vector<AbstractFilter*> filters){
    filters_ = filters;
}

void OrFilter::add_filter(AbstractFilter* filter){
    filters_.push_back(filter);
}

bool OrFilter::accept(PktInfo pkt_info){
    for (auto filter: filters_){
        if (filter->accept(pkt_info)){
            return true;
        }
    }
    if(filters_.size() == 0){
        return true;
    }
    return false;
}

void OrFilter::reset(){
    for (auto filter: filters_){
        filter->reset();
    }
}