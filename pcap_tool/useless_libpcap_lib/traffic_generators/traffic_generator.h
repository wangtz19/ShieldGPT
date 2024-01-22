#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include "abstract_traffic_generator.h"

#include <vector>

#include "utils.h"
#include "abstract_strategy.h"

class TrafficGenerator: public AbstractTrafficGenerator{
  public:
    TrafficGenerator(PktInfo template_pkt_info, timeval start_time = {0, 0}, timeval stop_time = {114514, 0}, std::vector<AbstractStrategy*> strategies = {});

    void add_strategy(AbstractStrategy* strategy);
    
    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;

  private:
    std::vector<AbstractStrategy*> strategies_;
    PktInfo template_pkt_info_;
    PktInfo curr_pkt_info_;
    timeval stop_time_;
};

#endif