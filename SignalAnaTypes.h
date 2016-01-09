#ifndef __SIGNALANATYPES__
#define __SIGNALANATYPES__

#include <vector>
#include <string>
#include <exception>
#include <fftw3.h>

namespace signalana {

  /// Message level 
  enum MessageLevel_t {
    kDEBUG,
    kINFO,
    kNORMAL,
    kWARNING,
    kERROR,
    kCRITICAL
  };

  /// fftw3 type for complex numbers
  typedef fftw_complex complex;


}

#endif  
