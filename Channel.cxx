#ifndef SIGNALANA_CHANNEL_CXX
#define SIGNALANA_CHANNEL_CXX

#include "Channel.h"

namespace signalana{

  void Channel::Clear(){

    _ch = 0;
    _samples = 0;
    _sampling_freq = 0;

    return;
  }

}

#endif
