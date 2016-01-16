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
#include "Channel.h"

/**
   \class Filter
   User defined class Filter ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Filter : public std::vector<double>,
                 public Channel {
    
  public:
    
    /// Default constructor
    Filter(){}

    /// Kernel constructor w/ freq-domain vector initializer
    Filter(const std::vector<double>& time_v);
    
    /// Default destructor
    ~Filter (){}

  private:
    
  };

}

#endif
/** @} */ // end of doxygen group 

