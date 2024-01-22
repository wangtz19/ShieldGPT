#ifndef ABSTRACT_FEATURE_EXTRACTOR_H
#define ABSTRACT_FEATURE_EXTRACTOR_H

#include "utils.h"

class AbstractFeatureExtractor{
  public:
    virtual void append_packet(PktInfo pkt_info) = 0;
    virtual void print_feature() = 0;
    virtual void reset() = 0;
};

#endif