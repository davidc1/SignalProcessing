/**
 * \file Channel.h
 *
 * \ingroup SignalProcessing
 * 
 * \brief Class def header for a class Channel
 *
 * @author david
 */

/** \addtogroup SignalProcessing

    @{*/
#ifndef SIGNALANA_CHANNEL_H
#define SIGNALANA_CHANNEL_H

#include <iostream>
#include "SignalAnaTypes.h"

/**
   \class Channel
   User defined class Channel ... these comments are used to generate
   doxygen documentation!
 */

namespace signalana{
  
  class Channel{
    
  public:
    
    /// Default constructor
    Channel(){}

    /// constructor w/ info
    Channel(const int& chnum, const int& samples, const double& sampling_freq){
      _ch = chnum;
      _samples = samples;
      _sampling_freq = sampling_freq;
    }

    /// Default destructor
    ~Channel(){}

    /// get channel number
    void setChannel(const int& ch) { _ch = ch; }
    
    /// get number of time-samples recorded
    void setSamples(const int& samples) { _samples = samples; }

    /// get sampling freq
    void setSamplingFrequency(const double& f) { _sampling_freq = f; }

    /// get channel number
    int getChannel() { return _ch; }
    
    /// get number of time-samples recorded
    int getSamples() { return _samples; }

    /// get sampling freq
    double getSamplingFrequency() { return _sampling_freq; }
    
    /// clear function
    void Clear();

  protected:

    int _ch;
    int _samples;
    int _sampling_freq;
    
    
    
  };

}

#endif
/** @} */ // end of doxygen group 

