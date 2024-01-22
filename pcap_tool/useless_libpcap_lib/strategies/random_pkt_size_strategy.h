#ifndef RANDOM_PKT_SIZE_STRATEGY_H
#define RANDOM_PKT_SIZE_STRATEGY_H

#include "abstract_strategy.h"

#include "utils.h"

class RandomPktSizeStrategy: public AbstractStrategy{
  public:
    RandomPktSizeStrategy(uint32_t min_size = 50, uint32_t max_size = 1500);

    PktInfo apply(PktInfo pkt_info) override;

  private:
    uint32_t min_size_;
    uint32_t max_size_;
};

#endif