#ifndef SIGNALANA_FILTERFACTORY_CXX
#define SIGNALANA_FILTERFACTORY_CXX

#include "FilterFactory.h"

namespace signalana{


  void FilterFactory::LoadVectorFilter(const std::vector<double>& filter_vals,
				       signalana::Filter& filter){

    // fft ticks
    int ticks = filter.getSamples()/2+1;
    
    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter_vals.size() != ticks)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    // check that the filter we are about to fill is also consistent
    if ((int)filter.size() != ticks)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int f=0; f < ticks; f++)
      filter[f] *= filter_vals[f];

    return;
  }


  void FilterFactory::GaussLowPassFilter(const double& mu, const double& sigma,
					 signalana::Filter& filter){

    // fft ticks
    int ticks = filter.getSamples()/2+1;
    // nyquist frequency
    double nyquist = filter.getSamplingFrequency()/2;
    // minimum frequency
    double minfreq = filter.getSamplingFrequency()*2/filter.getSamples();

    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter.size() != ticks)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int i=0; i < ticks; i++){
      // calculate the freq. corresponding to tick i
      double freq = (nyquist-minfreq)*(i/(float)ticks);
      // filter value at this frequency
      double filt = 1;
      if (freq < mu)
	filt = 1;
      else
	filt = exp( -0.5 * ((freq-mu)/sigma) * ((freq-mu)/sigma) );

      filter[i] *= filt;

      //std::cout << "[FilterFactory : GaussLowPassFiler] -> (mu,sigma) = (" << mu << ", " << sigma
      //		<< "), (tick, freq) ->  (" << i << ", " << freq << "), value : " << filt << std::endl;
    }// for all samples

    return;
  }


  void FilterFactory::GaussHighPassFilter(const double& mu, const double& sigma,
					 signalana::Filter& filter){

    // fft ticks
    int ticks = filter.getSamples()/2+1;
    // nyquist frequency
    double nyquist = filter.getSamplingFrequency()/2;
    // minimum frequency
    double minfreq = filter.getSamplingFrequency()*2/filter.getSamples();

    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter.size() != ticks)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int i=0; i < ticks; i++){
      // calculate the freq. corresponding to tick i
      double freq = (nyquist-minfreq)*(i/(float)ticks);
      // filter value at this frequency
      double filt = 1;
      if (freq > mu)
	filt = 1;
      else
	filt = exp( -0.5 * ((freq-mu)/sigma) * ((freq-mu)/sigma) );

      //std::cout << "[FilterFactory : GaussHighPassFiler] -> (mu,sigma) = (" << mu << ", " << sigma
      //		<< "), (tick, freq) ->  (" << i << ", " << freq << "), value : " << filt << std::endl;

      filter[i] *= filt;
    }// for all samples

    return;
  }


  void FilterFactory::ConstantFracFilter(const double& frac, const double& fmin,
					 const double& fmax, signalana::Filter& filter){

    // fft ticks
    int ticks = filter.getSamples()/2+1;
    // nyquist frequency
    double nyquist = filter.getSamplingFrequency()/2;
    // minimum frequency
    double minfreq = filter.getSamplingFrequency()*2/filter.getSamples();

    // first check that the vector size is consistent
    // with the number of time-samples being used
    if ((int)filter.size() != ticks)
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");

    for (int i=0; i < ticks; i++){
      // calculate the freq. corresponding to tick i
      double freq = (nyquist-minfreq)*(i/(float)ticks);
      
      // if out of region of interest -> continue
      if ( (freq < fmin) or (freq > fmax) )
	continue;

      filter[i] *= frac;

    }// for all samples

    return;
  }

}

#endif
