#ifndef RANDOM_INTER_PACKET_TIME_STRATEGY_H
#define RANDOM_INTER_PACKET_TIME_STRATEGY_H

#include "abstract_strategy.h"

#include <vector>

#include "utils.h"

class RandomInterPacketTimeStrategy: public AbstractStrategy{
  public:
    RandomInterPacketTimeStrategy(std::vector<timeval> inter_packet_times = {});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    std::vector<timeval> inter_packet_times_;
};

#endif