#ifndef AND_FILTER_H
#define AND_FILTER_H

#include "abstract_filter.h"

#include <vector>

class AndFilter: public AbstractFilter{
  public:
    AndFilter();
    AndFilter(std::vector<AbstractFilter*> filters);

    void add_filter(AbstractFilter* filter);

    bool accept(PktInfo pkt_info) override;
    void reset() override;
    
  private:
    std::vector<AbstractFilter*> filters_;
};

#endif