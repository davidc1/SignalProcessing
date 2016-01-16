#ifndef SIGNALANA_FILTER_CXX
#define SIGNALANA_FILTER_CXX

#include "Filter.h"

namespace signalana{

  Filter::Filter(const int& chnum, const int& samples, const double& sampling_freq)
    : Channel(chnum,samples,sampling_freq),
      std::vector<double>(samples/2+1,1.)
  {}
  
  Filter::Filter(const std::vector<double>& time_v)
    : std::vector<double>(time_v)
  {}

  Filter::Filter(const std::vector<double>& time_v, const int& ch)
    : std::vector<double>(time_v)
  {
    _ch = ch;
  }
    
    

}

#endif
