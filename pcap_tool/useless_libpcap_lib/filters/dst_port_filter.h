#ifndef DST_PORT_FILTER_H
#define DST_PORT_FILTER_H

#include "abstract_filter.h"

class DstPortFilter: public AbstractFilter{
  public:
    DstPortFilter(uint16_t port);
    DstPortFilter(uint16_t min_port, uint16_t max_port);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    uint16_t min_port_;
    uint16_t max_port_;
};

#endif