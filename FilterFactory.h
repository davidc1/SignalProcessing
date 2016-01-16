/**
 * \file FilterFactory.h
 *
 * \ingroup SignalProcessing
 * 
 * \brief Class def header for a class FilterFactory
 *
 * @author david caratelli
 */

/** \addtogroup SignalProcessing

    @{*/
#ifndef SIGNALANA_FILTERFACTORY_H
#define SIGNALANA_FILTERFACTORY_H

#include <iostream>
#include <vector>
#include "SignalAnaTypes.h"
#include "Filter.h"
#include <math.h>

/**
   \class FilterFactory
   User defined class FilterFactory ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class FilterFactory {
    
  public:
    
    /// Default constructor
    FilterFactory(){}
    
    /// Default destructor
    ~FilterFactory (){}

    
    /// filter producer from a vector
    void LoadVectorFilter(const std::vector<double>& filter_vals,
			  signalana::Filter& filter);

    /// gaussian-like low-pass filter
    void GaussLowPassFilter(const double& mu, const double& sigma,
			    signalana::Filter& filter);

    /// gaussian-like high-pass filter
    void GaussHighPassFilter(const double& mu, const double& sigma,
			    signalana::Filter& filter);
    
    /// constant fraction suppression in specific freq. range
    void ConstantFracFilter(const double& frac, const double& fmin,
			    const double& fmax, signalana::Filter& filter);

  };

}

#endif
/** @} */ // end of doxygen group 

