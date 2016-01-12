#ifndef LARLITE_DECO_CXX
#define LARLITE_DECO_CXX

#include "Deco.h"
#include "DataFormat/opdetwaveform.h"
#include "DataFormat/trigger.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

namespace larlite {

  Deco::Deco() {

    _fout = 0;
    _name = "Deco";
    _prod_name = "pmt_xmit";

  }

  bool Deco::initialize() {

    // initialize deconvolution tool
    _deco_tool.PrepareFFTW3(1501);
    _deco_tool.Initialize(1501);

    // load kernels
    std::vector< std::vector<double> > spe_kernels;
    TFile *f_kernel = new TFile(_spe_kernel_file.c_str());
    f_kernel->cd("specalib");
    for (int pmt=0; pmt < 32; pmt++){
      std::string hist_name_vals = Form("specalib/hSPE_ave_femch%02i",pmt);
      std::string hist_name_norm = Form("specalib/hSPE_norm_femch%02i",pmt);
      TH1D* hvals = (TH1D*)f_kernel->Get(hist_name_vals.c_str());
      TH1D* hnorm = (TH1D*)f_kernel->Get(hist_name_norm.c_str());
      std::vector<double> spe;
      for (int i=0; i < hvals->GetNbinsX(); i++)
	spe.push_back(hvals->GetBinContent(i+1)/hnorm->GetBinContent(i+1));
      spe_kernels.push_back(spe);
      delete hvals;
      delete hnorm;
    }
    _deco_tool.LoadKernels(spe_kernels);

    // load filters
    std::vector< std::vector<double> > wiener_filters;
    TFile *f_filter = new TFile(_wiener_filter_file.c_str());
    for (int pmt=0; pmt < 32; pmt++){
      TH1D* h = (TH1D*)f_filter->Get(Form("hFilter_pmt%02i",pmt));
      std::vector<double> filter(1501/2+1,0.);
      for (int i=0; i < h->GetNbinsX(); i++)
	filter[i] = h->GetBinContent(i+1);
      wiener_filters.push_back(filter);
      delete h;
    }
    _deco_tool.LoadFilters(wiener_filters);

    // reset time-profiling tools
    _decoLL_time.Reset();

    return true;
  }
  
  bool Deco::analyze(storage_manager* storage) {

    auto ev_wfs = storage->get_data<event_opdetwaveform>(_prod_name);

    auto ev_wfs_out = storage->get_data<event_opdetwaveform>("deconvolution");

    storage->set_id(ev_wfs->run(),ev_wfs->subrun(),ev_wfs->event_id());

    if (!ev_wfs){
      std::cout << "No pmt readout!" << std::endl;
      return true;
    }

    // apply deconvolution
    for (size_t n=0; n < ev_wfs->size(); n++){
      
      auto const& wf = ev_wfs->at(n);

      if ( (wf.ChannelNumber() >= 32) or (wf.size() < 500) )
	continue;

      _decoLL_time.Start();
      auto wfout = _deco_tool.Deconvolve(wf);
      _decoLL_time.Add(_decoLL_time.WallTime());
      ev_wfs_out->push_back(wfout);

    }// for all pmt waveforms
  
  
    return true;
  }

  bool Deco::finalize() {

    std::cout << "Time for each LL Deconvolution function call : " << _decoLL_time.ReportTime()*1.e6 << " [usec]" << std::endl;
    std::cout << "Time for each Deconvolution function call    : " << _deco_tool.ReportTime()*1.e6 << " [usec]" << std::endl;

    _deco_tool.writeKernels(_fout);
    _deco_tool.writeFilters(_fout);

  
    return true;
  }

}
#endif
