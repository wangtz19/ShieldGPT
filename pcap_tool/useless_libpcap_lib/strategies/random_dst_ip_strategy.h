#ifndef RANDOM_DST_IP_STRATEGY_H
#define RANDOM_DST_IP_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomDstIpStrategy: public AbstractStrategy{
  public:
    RandomDstIpStrategy(std::vector<uint32_t> dst_ips = {});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<uint32_t> dst_ips_;
};

#endif