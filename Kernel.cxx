#ifndef SIGNALANA_KERNEL_CXX
#define SIGNALANA_KERNEL_CXX

#include "Kernel.h"

namespace signalana{

  void Kernel::Clear(){
    
    _time_array.clear();
    _channel = -1;

    return;
  }

}

#endif
