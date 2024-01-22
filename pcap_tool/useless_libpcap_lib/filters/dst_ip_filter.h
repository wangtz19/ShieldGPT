#ifndef DST_IP_FILTER_H
#define DST_IP_FILTER_H

#include "abstract_filter.h"

class DstIpFilter: public AbstractFilter{
  public:
    DstIpFilter(char* prefix_str, uint32_t prefix_len = 32);
    DstIpFilter(uint32_t prefix, uint32_t prefix_len = 32);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    uint32_t prefix_;
    uint32_t mask_;
};

#endif