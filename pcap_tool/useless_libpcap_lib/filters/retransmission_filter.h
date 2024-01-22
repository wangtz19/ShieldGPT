#ifndef RETRANSMISSION_FILTER_H
#define RETRANSMISSION_FILTER_H

#include "abstract_filter.h"

#include <set>
#include <map>

class RetransmissionFilter: public AbstractFilter{
  public:
    RetransmissionFilter();

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    std::set<int64_t> pkt_hash_set_;
    std::map<int64_t, FiveTuple> double_check_map_;
};

#endif