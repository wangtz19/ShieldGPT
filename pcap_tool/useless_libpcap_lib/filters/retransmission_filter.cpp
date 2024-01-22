#include "retransmission_filter.h"

#include <iostream>

#include "utils.h"

RetransmissionFilter::RetransmissionFilter(){

}

bool RetransmissionFilter::accept(PktInfo pkt_info){
    auto iter = pkt_hash_set_.find(pkt_info.pkt_hash);
    if(iter == pkt_hash_set_.end()){
        pkt_hash_set_.insert(pkt_info.pkt_hash);
        double_check_map_.insert(std::pair<int64_t, FiveTuple>(pkt_info.pkt_hash, pkt_info.flow_id));
        return true;
    }
    else {
        if(double_check_map_[pkt_info.pkt_hash] == pkt_info.flow_id){
            return false;
        }
        else {
            std::cout << "Error from retransmission filter: pkt_hash collision " << double_check_map_[pkt_info.pkt_hash] << " " << pkt_info.flow_id << std::endl;
            return false;
        }
    }
}

void RetransmissionFilter::reset(){
    pkt_hash_set_.clear();
    double_check_map_.clear();
}