#ifndef CARPET_BOMBING_STRATEGY_H
#define CARPET_BOMBING_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

#define POLLING_STRATEGY_RANDOM 0
#define POLLING_STRATEGY_ROUND_ROBIN 1

class CarpetBombingStrategy: public AbstractStrategy{
  public:
    CarpetBombingStrategy(std::vector<uint32_t> target_ip_list, uint32_t polling_strategy = POLLING_STRATEGY_ROUND_ROBIN);

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<uint32_t> target_ip_list_;
    uint32_t polling_strategy_;
    uint32_t pkt_counter_;
};

#endif