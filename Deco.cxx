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
    _prod_name = "pmtreadout";
    _Nticks_beam = 2048;
    _Nticks_cosmic = 256;
    _spe_kernel_file = "";
    _wiener_filter_file_beam = "";
    _wiener_filter_file_cosmic = "";

  }

  bool Deco::initialize() {
    
    // initialize deconvolution tool
    // cosmic window
    _deco_tool_cosmic.PrepareFFTW3();
    _deco_tool_cosmic.Initialize(_Nticks_cosmic);
    // beam-gate window
    _deco_tool_beam.PrepareFFTW3();
    _deco_tool_beam.Initialize(_Nticks_beam);

    // load kernels
    std::cout << "loading kernels..." << std::endl;
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
      _deco_tool_beam.LoadKernel(pmt,spe);
      _deco_tool_cosmic.LoadKernel(pmt,spe);
      delete hvals;
      delete hnorm;
    }
    delete f_kernel;
    std::cout << "...done" << std::endl;

    // load filters
    std::cout << "loading filters..." << std::endl;
    // cosmic window
    TFile *f_filter_cosmic = new TFile(_wiener_filter_file_cosmic.c_str());
    for (int pmt=0; pmt < 32; pmt++){
      TH1D* h = (TH1D*)f_filter_cosmic->Get(Form("hFilterCosmicDiscriminator_pmt%02i",pmt));
      std::vector<double> filter(_Nticks_cosmic/2+1,0.);
      for (int i=0; i < h->GetNbinsX(); i++)
	filter[i] = h->GetBinContent(i+1);
      _filter_factory.LoadVectorFilter(filter,_deco_tool_cosmic.EditableFilter(pmt));
      _filter_factory.GaussHighPassFilter(0.5e6,0.8e6,_deco_tool_beam.EditableFilter(pmt));
      delete h;
    }
    delete f_filter_cosmic;
    // beam-gate window
    TFile *f_filter_beam = new TFile(_wiener_filter_file_beam.c_str());
    for (int pmt=0; pmt < 32; pmt++){
      _deco_tool_beam.LoadFilter(pmt);
      TH1D* h = (TH1D*)f_filter_beam->Get(Form("hFilter_pmt%02i",pmt));
      std::vector<double> filter(_Nticks_beam/2+1,0.);
      for (int i=0; i < h->GetNbinsX(); i++)
	filter[i] = h->GetBinContent(i+1);
      //_filter_factory.LoadVectorFilter(filter,_deco_tool_beam.EditableFilter(pmt));
      //_filter_factory.ConstantFracFilter(0.5,0,0.5e6,_deco_tool_beam.EditableFilter(pmt));
      _filter_factory.GaussHighPassFilter(0.5e6,0.3e6,_deco_tool_beam.EditableFilter(pmt));
      _filter_factory.GaussLowPassFilter(10e6,5e6,_deco_tool_beam.EditableFilter(pmt));
      delete h;
    }
    delete f_filter_beam;
    std::cout << "...done" << std::endl;

    // deconvolve the kernels to have a record
    _deco_tool_beam.deconvolveKernels(_fout);
    _deco_tool_cosmic.deconvolveKernels(_fout);

    // reset time-profiling tools
    _decoLL_time_beam.Reset();
    _decoLL_time_cosmic.Reset();

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

      if ( wf.ChannelNumber() >= 32 )
	continue;

      // beam-gate waveforms
      if (wf.size() > 256){
	_decoLL_time_beam.Start();
	auto wfout = _deco_tool_beam.Deconvolve(wf);
	_decoLL_time_beam.Add(_decoLL_time_beam.WallTime());
	ev_wfs_out->push_back(wfout);
      }
      // cosmic discriminator waveforms
      else {
	_decoLL_time_cosmic.Start();
	auto wfout = _deco_tool_cosmic.Deconvolve(wf);
	_decoLL_time_cosmic.Add(_decoLL_time_cosmic.WallTime());
	ev_wfs_out->push_back(wfout);
      }

    }// for all pmt waveforms
  
  
    return true;
  }

  bool Deco::finalize() {

    _deco_tool_beam.ResetFFTW3();
    _deco_tool_cosmic.ResetFFTW3();

    std::cout << "Time for each LL Deconvolution function call [beam]   : " << _decoLL_time_beam.ReportTime()*1.e6 << " [usec]" << std::endl;
    std::cout << "Time for each Deconvolution function call    [beam]   : " << _deco_tool_beam.ReportTime()*1.e6 << " [usec]" << std::endl;
    std::cout << "Time for each LL Deconvolution function call [cosmic] : " << _decoLL_time_cosmic.ReportTime()*1.e6 << " [usec]" << std::endl;
    std::cout << "Time for each Deconvolution function call    [cosmic] : " << _deco_tool_cosmic.ReportTime()*1.e6 << " [usec]" << std::endl;

    _deco_tool_beam.writeKernels(_fout);
    _deco_tool_cosmic.writeKernels(_fout);
    _deco_tool_beam.writeFilters(_fout);
    _deco_tool_cosmic.writeFilters(_fout);

  
    return true;
  }

}
#endif
