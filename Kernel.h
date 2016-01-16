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
#include "Channel.h"

/**
   \class Kernel
   User defined class Kernel ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Kernel : public std::vector<double>,
                 public Channel {
    
  public:
    
    /// Default constructor
    Kernel(){}

    /// Kernel constructor w/ time-domain vector initializer
    Kernel(const std::vector<double>& time_v);
    
    /// Default destructor
    ~Kernel(){}

    /**
       @brief clear Kernel information
     */
    void Clear();

    /**
       @brief fill freq-space information for kernel
     */
    void setFFT(const fftw_complex *fft,const int& N);

    /**
       @brief getter for kernel's fft values
     */
    double real(const int& n);
    double imag(const int& n);

  private:
    
    // freq-domain kernel information
    complex *_freq_space;
    
  };

}

#endif
/** @} */ // end of doxygen group 

