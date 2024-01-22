#ifndef RANDOM_SRC_IP_STRATEGY_H
#define RANDOM_SRC_IP_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomSrcIpStrategy: public AbstractStrategy{
  public:
    RandomSrcIpStrategy(std::vector<uint32_t> src_ips = {});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<uint32_t> src_ips_;
};

#endif