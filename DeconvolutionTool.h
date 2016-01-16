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
    DeconvolutionTool();

    /// Default constructor
    DeconvolutionTool(const int& num_chan, const int& N, const double& sampling_freq);
    
    /// Default destructor
    ~DeconvolutionTool(){}

    /**
       @brief Prepare FFTW3
       @detail This function prepares the tools needed by the FFTW3
       package. Given N, the number of ticks in the input
       signal, a plan for ft and i-ft are prepared for future use
     */
    void PrepareFFTW3();

    /**
       @brief Reset FFTW3
       @detail function to clear memory and reset all tools used
       by fftw3
     */
    void ResetFFTW3();

    /**
    @brief Apply Deconvolution
    @detail Take input signal and after checking the vector size is
    appropriate then apply the deconvolution.
    */
    double* Deconvolve(const std::vector<double>& sig_v, const size_t& channel);

    /**
       @brief Prepare Kernel vector
       @detail function to load kernels for the various channels
     */
    void LoadKernels(const std::vector<std::vector<double> >& kernel_v);

    /**
       @brief Prepare individual kernel
     */
    void LoadKernel(const int& chnum, const std::vector<double>& kernel_vals);

    /**
       @brief return editable kernel
    */
    signalana::Kernel& EditableKernel(const int& ch) { return _kernel_v[ch]; }

    /**
       @brief Prepare single filter (filter everywhere == 1 after this call)
     */
    void LoadFilter(const int& chnum);

    /**
       @brief return editable filter
    */
    signalana::Filter& EditableFilter(const int& ch) { return _filter_v[ch]; }


    /**
       @brief Prepare Filter vector
       @detail function to load kernels for the various channels
     */
    void LoadFilters(const std::vector<std::vector<double> >& filter_v);


    /**
       @brief Load normalization constants for deconvolution
     */
    void LoadNormalizations(const std::vector<double>& norm_v) { _normalization_v = norm_v; }

    /**
     @brief setter for debug mode
     */
    void setDebug(signalana::MessageLevel_t debug) { _debug = debug; }

    /**
       @brief kernel getter
    */
    const signalana::Kernel& kernel(const int& n);

    /**
       @brief filter getter
    */
    const signalana::Filter& filter(const int& n);

    /**
       @brief tick number getter
    */
    int N() { return _N; }

    /**
       @brief set name of this class
     */
    void setName(std::string name) { _name = name; }

    /**
       @brief module name gtter
    */
    std::string name() { return _name; }

    /**
       @brief set padding value
    */
    void setPadding(double p) { _padding = p; }

    /**
       @brief set the number of channels
     */
    void setNumChannels(const int& n) { _num_channels = n; }


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
    // _processed -> the output time-domain signal
    // after processing
    double *_processed;

    // the size of the signal expected by this instance of DeconvolutionTool
    int _N;

    // padding -> the amount by which to increase the kernel's FFT magnitude
    // when deconvolving
    double _padding;

    // number of channels expected
    int _num_channels;

    // sampling frequency
    double _sampling_freq;

    // vector of kernels to be used by the tool instance
    std::vector<signalana::Kernel> _kernel_v;

    // vector of filters to be used by the tool instance
    std::vector<signalana::Filter> _filter_v;

    // vector of per-ch normalization factors
    // these are to be applied so that deconvolved pulses
    // preserve the signal area
    std::vector<double> _normalization_v;

    // debug variable
    signalana::MessageLevel_t _debug;

    // instance of timer class to profile
    signalana::Watch _watch;

    // a name to specify the instance of this class
    std::string _name;
    
  };

}



#endif
/** @} */ // end of doxygen group 

