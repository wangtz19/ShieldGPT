#include "statistic_feature.h"

std::ostream& operator<<(std::ostream& os, const StatisticFeature& statistic_feature){
    os << "\"total_packet_count:\"" << statistic_feature.total_packet_count << ",";
    os << "\"total_packet_bytes:\"" << statistic_feature.total_packet_bytes << ",";
    os << "\"total_squared_packet_length:\"" << statistic_feature.total_squared_packet_length << ",";
    os << "\"min_packet_length:\"" << statistic_feature.min_packet_length << ",";
    os << "\"max_packet_length:\"" << statistic_feature.max_packet_length << ",";
    os << "\"total_inter_packet_time:\"" << statistic_feature.total_inter_packet_time << ",";
    os << "\"total_squared_inter_packet_time:\"" << statistic_feature.total_squared_inter_packet_time << ",";
    os << "\"min_inter_packet_time:\"" << statistic_feature.min_inter_packet_time << ",";
    os << "\"max_inter_packet_time:\"" << statistic_feature.max_inter_packet_time;
    return os;
}