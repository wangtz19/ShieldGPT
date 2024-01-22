#ifndef RANDOM_SRC_PORT_STRATEGY_H
#define RANDOM_SRC_PORT_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomSrcPortStrategy: public AbstractStrategy{
  public:
    RandomSrcPortStrategy(std::vector<uint16_t> src_ports = {});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<uint16_t> src_ports_;
};

#endif