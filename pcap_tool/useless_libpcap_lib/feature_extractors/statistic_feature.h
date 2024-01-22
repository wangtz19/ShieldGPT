#ifndef STATISTIC_FEATURE_H
#define STATISTIC_FEATURE_H

#include <ostream>

struct StatisticFeature{
    uint64_t total_packet_count;
    uint64_t total_packet_bytes;
    uint64_t total_squared_packet_length;
    uint64_t min_packet_length;
    uint64_t max_packet_length;
    uint64_t total_inter_packet_time;
    uint64_t total_squared_inter_packet_time;
    uint64_t min_inter_packet_time;
    uint64_t max_inter_packet_time;

    friend std::ostream& operator<<(std::ostream& os, const StatisticFeature& statistic_feature);
};

#endif