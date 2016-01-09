/**
 * \file DeconvolutionTool.h
 *
 * \ingroup SignalProcessing
 * 
 * \brief Class def header for a class DeconvolutionTool
 *
 * @author david caratelli
 */

/** \addtogroup SignalProcessing

    @{*/
#ifndef SIGNALANA_DECONVOLUTIONTOOL_H
#define SIGNALANA_DECONVOLUTIONTOOL_H

#include <iostream>
#include "Kernel.h"
#include "Filter.h"
#include "SignalAnaTypes.h"

/**
   \class DeconvolutionTool
   User defined class DeconvolutionTool ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class DeconvolutionTool{
    
  public:
    
    /// Default constructor
    DeconvolutionTool(){}
    
    /// Default destructor
    ~DeconvolutionTool(){}


  private:

    // the deconvolution utility must contain two fftw 'plans'
    // these plans will be used to execute forward and inverse
    // DFTs to obtain complex FTs from real discrete signals
    // and vice versa.
    fftw_plan _forward_fft;
    fftw_plan _inverse_fft;

    // the FFT plans to be executed need arrays on which to act
    // arrays are initialized meeting the requirements of the
    // fftw3 package
    
    // _signal -> the original signal that is recorded and
    // needs to be analyzed
    double *_signal;
    // _sigfft -> the fft of the original signal
    complex *_sigfft;
    
  };

}



#endif
/** @} */ // end of doxygen group 

