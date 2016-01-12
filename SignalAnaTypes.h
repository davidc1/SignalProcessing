#ifndef __SIGNALANATYPES__
#define __SIGNALANATYPES__

#include <vector>
#include <string>
#include <exception>
#include <complex.h>
#include <fftw3.h>
#include <sys/time.h>
#include <time.h>

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

  
  /// time class for time profiling
  class Watch {
  public:
    Watch(){}
    ~Watch(){}
    
    void Start() {
      // Get current wall time
      struct timeval current_time;
      gettimeofday(&current_time,NULL);
      _wall_time_start = (double)current_time.tv_sec + (double)current_time.tv_usec * 1.e-6;
      // Get current cpu time
      _cpu_time_start = (double)(clock());
    }
    double WallTime() {
      // Get current wall time
      struct timeval current_time;
      gettimeofday(&current_time,NULL);
      double now = (double)current_time.tv_sec + (double)current_time.tv_usec * 1.e-6;
      // Return diff
      return (now - _wall_time_start);
    }
    double CPUTime() {
      // Get cpu time
      double now = (double)(clock());
      // Return diff
      return (now - _cpu_time_start)/CLOCKS_PER_SEC;
    }
    void Reset(){
      _cpu_time_start = 0;
      _wall_time_start = 0;
      _tot_time = 0;
      _tot_calls = 0;
    }
    void Add(double t){
      _tot_time  += t;
      _tot_calls += 1;
    }

    double ReportTime(){
      return _tot_time/_tot_calls;
    }
    
  private:
    double _cpu_time_start;
    double _wall_time_start;
    double _tot_time;
    int    _tot_calls;
  };
  
}

#endif  
