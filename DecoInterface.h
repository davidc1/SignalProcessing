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
  DecoInterface() : DeconvolutionTool() {}
    
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

    /// function to report profiling time
    double ReportTime() { return _deco_time.ReportTime(); }

  protected:

    // waveform to store the signal that will be processed by base class
    std::vector<double> _signal;

    // a TFile where to store the kernel / filter wfs and ffts
    TFile* _fout;

    // time profiling tool
    signalana::Watch _deco_time;
    
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
