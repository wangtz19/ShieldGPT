#ifndef ABSTRACT_TRAFFIC_GENERATOR_H
#define ABSTRACT_TRAFFIC_GENERATOR_H

#include "utils.h"

class AbstractTrafficGenerator {
  public:
    virtual PktInfo get_current_pkt_info() = 0; // get current pkt info
    virtual timeval generate_next() = 0; // generate next valid pkt time, if is_end() return timeval()
    virtual bool is_end() = 0; // is end of traffic generator
    virtual void close() = 0; // close traffic generator
};

#endif