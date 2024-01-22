#ifndef OR_FILTER_H
#define OR_FILTER_H

#include "abstract_filter.h"

#include <vector>

class OrFilter: public AbstractFilter{
  public:
    OrFilter();
    OrFilter(std::vector<AbstractFilter*> filters);
    
    void add_filter(AbstractFilter* filter);

    bool accept(PktInfo pkt_info) override;
    void reset() override;

  private:
    std::vector<AbstractFilter*> filters_;
};

#endif