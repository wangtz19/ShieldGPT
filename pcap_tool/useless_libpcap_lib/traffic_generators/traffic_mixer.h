#ifndef TRAFFIC_MIXER_H
#define TRAFFIC_MIXER_H

#include <vector>
#include <queue>

#include "utils.h"
#include "abstract_traffic_generator.h"

struct TrafficMixerEvent {
    timeval time;
    uint32_t traffic_generator_index;

    bool operator<(const TrafficMixerEvent& a) const{
        if(time.tv_sec != a.time.tv_sec)return time.tv_sec >= a.time.tv_sec;
        return time.tv_usec >= a.time.tv_usec;
    }

    TrafficMixerEvent(){}
    TrafficMixerEvent(timeval time, int traffic_generator_index)
        : time(time), 
          traffic_generator_index(traffic_generator_index){
    }
};

class TrafficMixer: public AbstractTrafficGenerator{
  public:
    TrafficMixer();

    void add_traffic_generator(AbstractTrafficGenerator* traffic_generator);
    
    PktInfo get_current_pkt_info() override;
    timeval generate_next() override;
    bool is_end() override;
    void close() override;
  
  private:
    std::vector<AbstractTrafficGenerator*> traffic_generators_;
    std::priority_queue<TrafficMixerEvent> event_queue_;
};

#endif