/**
 * \file DecoInterface.h
 *
 * \ingroup Playground
 * 
 * \brief Class def header for a class DecoInterface
 *
 * @author david caratelli
 */

/** \addtogroup Playground

    @{*/

#ifndef SIGNALANA_DECOINTERFACE_H
#define SIGNALANA_DECOINTERFACE_H

#include "DeconvolutionTool.h"
#include "DataFormat/opdetwaveform.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

namespace signalana {
  /**
     \class DecoInterface
     User custom analysis class made by SHELL_USER_NAME
   */
  class DecoInterface : public signalana::DeconvolutionTool{
  
  public:

    /// Default constructor
  DecoInterface() : DeconvolutionTool() { _baseline = 2048; _pmt_num = 32; }

    /// Default constructor
  DecoInterface(const int& num_chan, const int& N, const double& sampling_freq)
    : DeconvolutionTool(num_chan,N,sampling_freq)
      { _baseline = 2048; _pmt_num = 32; }
    
    /// Default destructor
    virtual ~DecoInterface() {}
    
    /// initialize tool
    void Initialize(const int& n);

    /// deconvolve function
    ::larlite::opdetwaveform Deconvolve(const ::larlite::opdetwaveform& wf);

    /// function to be used to save kernels / filters to output root file
    void writeKernels(TFile* _fout);
    
    /// function to be used to save filters to output root file
    void writeFilters(TFile* _fout);

    /// function to deconvolve kernels and save them
    void deconvolveKernels(TFile* _fout);

    /// function to report profiling time
    double ReportTime() { return _deco_time.ReportTime(); }

    /// set baseline
    void setBaseline(double d) { _baseline = d; }

    /// set number of PMTs
    void setPMTNum(int n) { _pmt_num = n; }


  protected:

    // waveform to store the signal that will be processed by base class
    std::vector<double> _signal;

    // a TFile where to store the kernel / filter wfs and ffts
    TFile* _fout;

    // time profiling tool
    signalana::Watch _deco_time;

    // baseline to be subtracted from signals
    double _baseline;
    
    // number of PMTs
    int _pmt_num;

    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
