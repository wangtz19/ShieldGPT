#ifndef PROTO_FILTER_H
#define PROTO_FILTER_H

#include "abstract_filter.h"

class ProtoFilter: public AbstractFilter{
  public:
    ProtoFilter(uint8_t proto);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    uint8_t proto_;
};

#endif