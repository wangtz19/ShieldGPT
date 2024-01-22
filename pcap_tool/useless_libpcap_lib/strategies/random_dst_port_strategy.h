#ifndef RANDOM_DST_PORT_STRATEGY_H
#define RANDOM_DST_PORT_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomDstPortStrategy: public AbstractStrategy{
  public:
    RandomDstPortStrategy(std::vector<uint16_t> dst_ports = {});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<uint16_t> dst_ports_;
};

#endif