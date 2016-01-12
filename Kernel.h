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
#include "SignalAnaTypes.h"

/**
   \class Kernel
   User defined class Kernel ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Kernel : public std::vector<double>{
    
  public:
    
    /// Default constructor
    Kernel(){}

    /// Kernel constructor w/ time-domain vector initializer
    Kernel(const std::vector<double>& time_v);
    
    /// Default destructor
    ~Kernel(){}

    /**
       Function to clear object
     */
    void Clear();

    /**
       @brief Set channel for this kernel
    */
    void setChannel(size_t ch) { _channel = ch; }

    /**
       @brief fill freq-space information for kernel
     */
    void setFFT(const fftw_complex *fft,const int& N);

    /**
       @brief getter for kernel's fft values
     */
    const double real(const int& n);
    const double imag(const int& n);

  private:
    
    // channel this kernel is for
    size_t _channel;
    // freq-domain kernel information
    fftw_complex *_freq_space;
    // fft size:
    int _N;
    //std::vector<fftw_complex> _freq_space;
    
  };

}

#endif
/** @} */ // end of doxygen group 

