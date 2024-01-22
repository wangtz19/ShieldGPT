#ifndef RANDOM_ALL_STRATEGY_H
#define RANDOM_ALL_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomAllStrategy: public AbstractStrategy{
  public:
    RandomAllStrategy();

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<AbstractStrategy*> strategies_;
};

#endif