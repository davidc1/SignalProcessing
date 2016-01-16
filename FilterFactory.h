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
    FilterFactory(){ _samples=1; _sampling_rate=1; }
    
    /// constructor w/ input for filter parameters
    FilterFactory(const int& samples, const double& sampling_rate);
    
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

    

  private:
    
    // two fundamental quantities needed to produce a filter:
    // freq sampling and number of ticks samples
    int _samples;
    double _sampling_rate; // Hz
    // nyquist frequency: maximum frequency for the specified sampling rate
    double _nyquist;
    // minimum frequency, given the sampling rate and number of time-samples recorded
    double _minfreq;
    
  };

}

#endif
/** @} */ // end of doxygen group 

