/**
 * \file Filter.h
 *
 * \ingroup SignalProcessing
 * 
 * \brief Class def header for a class Filter
 *
 * @author david caratelli
 */

/** \addtogroup SignalProcessing

    @{*/
#ifndef SIGNALANA_FILTER_H
#define SIGNALANA_FILTER_H

#include <iostream>
#include <vector>

/**
   \class Filter
   User defined class Filter ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Filter{
    
  public:
    
    /// Default constructor
    Filter(){}
    
    /// Default destructor
    ~Filter (){}

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

