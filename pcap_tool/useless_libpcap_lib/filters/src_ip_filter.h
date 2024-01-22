#ifndef SRC_IP_FILTER_H
#define SRC_IP_FILTER_H

#include "abstract_filter.h"

class SrcIpFilter: public AbstractFilter{
  public:
    SrcIpFilter(char* prefix_str, uint32_t prefix_len = 32);
    SrcIpFilter(uint32_t prefix, uint32_t prefix_len = 32);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    uint32_t prefix_;
    uint32_t mask_;
};

#endif