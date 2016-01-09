/**
 * \file Kernel.h
 *
 * \ingroup SignalProcessing
 * 
 * \brief Class def header for a class Kernel
 *
 * @author david caratelli
 */

/** \addtogroup SignalProcessing

    @{*/
#ifndef SIGNALANA_KERNEL_H
#define SIGNALANA_KERNEL_H

#include <iostream>
#include <vector>

/**
   \class Kernel
   User defined class Kernel ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Kernel{
    
  public:
    
    /// Default constructor
    Kernel(){}
    
    /// Default destructor
    ~Kernel(){}

    /**
       Function to clear object
     */
    void Clear();

  private:
    
    // channel this kernel is for
    size_t _channel;
    // time-domain kernel information
    std::vector<double> _time_array;
    // freq-domain kernel information
    
    
  };

}

#endif
/** @} */ // end of doxygen group 

