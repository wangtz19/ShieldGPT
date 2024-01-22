#ifndef CONST_INTER_PACKET_TIME_STRATEGY_H
#define CONST_INTER_PACKET_TIME_STRATEGY_H

#include "abstract_strategy.h"

#include "utils.h"

class ConstInterPacketTimeStrategy: public AbstractStrategy{
  public:
    ConstInterPacketTimeStrategy(timeval inter_packet_time = {0, 1});

    PktInfo apply(PktInfo pkt_info) override;

  private:
    timeval inter_packet_time_;
};

#endif