#ifndef FIVE_TUPLE_FILTER_H
#define FIVE_TUPLE_FILTER_H

#include "abstract_filter.h"
#include "and_filter.h"

class FiveTupleFilter: public AbstractFilter{
  public:
    FiveTupleFilter(char* src_ip, char* dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t proto);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    AndFilter filter_;
};

#endif