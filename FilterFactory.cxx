#ifndef SIGNALANA_FILTERFACTORY_CXX
#define SIGNALANA_FILTERFACTORY_CXX

#include "FilterFactory.h"

namespace signalana{

  FilterFactory::FilterFactory(const int& samples, const double& sampling_rate){

    _sampling_rate = sampling_rate;
    _samples       = samples;
    _nyquist       = _sampling_rate/2.;
    _minfreq       = 2*_sampling_rate/_samples;

  }


  void FilterFactory::LoadVectorFilter(const std::vector<double>& filter_vals,
				       signalana::Filter& filter){
    
    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter_vals.size() != _samples/2+1)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    // check that the filter we are about to fill is also consistent
    if ((int)filter.size() != _samples/2+1)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int f=0; f < _samples/2+1; f++)
      filter[f] *= filter_vals[f];

    return;
  }


  void FilterFactory::GaussLowPassFilter(const double& mu, const double& sigma,
					 signalana::Filter& filter){

    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter.size() != _samples/2+1)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int i=0; i < _samples/2+1; i++){
      // calculate the freq. corresponding to tick i
      double freq = (_nyquist-_minfreq)*(i/(_samples/2.+1));
      // filter value at this frequency
      double filt = 1;
      if (freq < mu)
	filt = 1;
      else
	filt = exp( -0.5 * ((freq-mu)/sigma) * ((freq-mu)/sigma) );

      filter[i] *= filt;
    }// for all samples

    return;
  }


  void FilterFactory::GaussHighPassFilter(const double& mu, const double& sigma,
					 signalana::Filter& filter){

    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter.size() != _samples/2+1)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int i=0; i < _samples/2+1; i++){
      // calculate the freq. corresponding to tick i
      double freq = (_nyquist-_minfreq)*(i/(_samples/2.+1));
      // filter value at this frequency
      double filt = 1;
      if (freq > mu)
	filt = 1;
      else
	filt = exp( -0.5 * ((freq-mu)/sigma) * ((freq-mu)/sigma) );

      filter[i] *= filt;
    }// for all samples

    return;
  }

}

#endif
