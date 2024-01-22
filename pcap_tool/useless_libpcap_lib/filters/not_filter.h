#ifndef NOT_FILTER_H
#define NOT_FILTER_H

#include "abstract_filter.h"

class NotFilter: public AbstractFilter{
  public:
    NotFilter(AbstractFilter* filter);

    bool accept(PktInfo pkt_info) override;
    void reset() override;
    
  private:
    AbstractFilter* filter_;
};

#endif