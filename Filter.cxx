#ifndef SIGNALANA_FILTER_CXX
#define SIGNALANA_FILTER_CXX

#include "Filter.h"

namespace signalana{

  void Filter::Clear(){
    
    _time_array.clear();
    _channel = -1;

    return;
  }

}

#endif
