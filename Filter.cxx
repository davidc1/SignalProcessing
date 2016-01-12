#ifndef SIGNALANA_FILTER_CXX
#define SIGNALANA_FILTER_CXX

#include "Filter.h"

namespace signalana{

  Filter::Filter(const std::vector<double>& time_v)
    : std::vector<double>(time_v)
  {}

  void Filter::Clear(){
    
    _channel = -1;

    return;
  }

}

#endif
