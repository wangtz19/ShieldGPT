#ifndef ABSTRACT_STRATEGY_H
#define ABSTRACT_STRATEGY_H

#include "utils.h"

class AbstractStrategy{
  public:
    virtual PktInfo apply(PktInfo pkt_info) = 0;
};

#endif