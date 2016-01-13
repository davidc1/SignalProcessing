/**
 * \file Deco.h
 *
 * \ingroup Playground
 * 
 * \brief Class def header for a class Deco
 *
 * @author david caratelli
 */

/** \addtogroup Playground

    @{*/

#ifndef LARLITE_DECO_H
#define LARLITE_DECO_H

#include "Analysis/ana_base.h"
#include "DecoInterface.h"
#include "SignalAnaTypes.h"

namespace larlite {
  /**
     \class Deco
     User custom analysis class made by SHELL_USER_NAME
   */
  class Deco : public ana_base{
  
  public:

    /// Default constructor
    Deco();

    /// Default destructor
    virtual ~Deco(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    // set producer name for fifo data-product
    void setProducer(std::string s) { _prod_name = s; }
    
    // set file path for spe kernels
    void setSPEKernelFile(std::string s) { _spe_kernel_file = s; }

    // set file path for filters
    void setFilterFile(std::string s) { _wiener_filter_file = s; }

    // set the number of ticks to be used for the FFT
    void setNumTicks(int n) { _Nticks = n; }

  protected:

    signalana::DecoInterface _deco_tool;

    // producer name of fifo data-product
    std::string _prod_name;

    // file where to find single PE kernels
    std::string _spe_kernel_file;

    // file where to find filters
    std::string _wiener_filter_file;

    // size of array to be used for FFT computations
    int _Nticks;

    // time profiling tool
    signalana::Watch _decoLL_time;

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
