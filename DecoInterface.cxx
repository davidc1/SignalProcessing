#ifndef SIGNALANA_DECOINTERFACE_CXX
#define SIGNALANA_DECOINTERFACE_CXX

#include "DecoInterface.h"

namespace signalana {

  void DecoInterface::Initialize(const int& n){
    _signal.resize(n);
    _deco_time.Reset();
  }

  ::larlite::opdetwaveform DecoInterface::Deconvolve(const ::larlite::opdetwaveform& wf){

    // copy vector of shorts into a vector of doubles
    std::vector<double> signal(N(),0.);
    for (size_t n=0; n < wf.size(); n++)
      signal[n] = (double)wf[n]-_baseline;
    
    _deco_time.Start();
    auto processed_wf = ::signalana::DeconvolutionTool::Deconvolve(signal,(size_t)wf.ChannelNumber());
    _deco_time.Add(_deco_time.WallTime());

    ::larlite::opdetwaveform wfout;
    for (int n=0; n < wf.size(); n++)
      wfout.push_back((short)processed_wf[n]);
    wfout.SetChannelNumber(wf.ChannelNumber());
    wfout.SetTimeStamp(wf.TimeStamp());

    return wfout;
  }

  void DecoInterface::writeKernels(TFile* _fout){

    _fout->cd();

    for (int pmt=0; pmt < _pmt_num; pmt++){
      
      // kernel object for this pmt
      auto k = kernel(pmt);

      // save time-domain kernel wf
      int size = k.size();
      TH1D* hKernelTime = new TH1D(Form("hKernelTime_%s_%02i",name().c_str(),pmt),Form("kernel for pmt %02i",pmt),size,0,size);
      for (int n=0; n < size; n++)
	hKernelTime->SetBinContent(n+1,k[n]);
      hKernelTime->Write();
      delete hKernelTime;

      // save freq-domain kernel info
      int nfreq = N()/2+1;
      TH1D* hKernelFreqReal = new TH1D(Form("hKernelFreqReal_%s_%02i",name().c_str(),pmt),Form("kernel for pmt %02i",pmt),nfreq,0,nfreq);
      TH1D* hKernelFreqImag = new TH1D(Form("hKernelFreqImag_%s_%02i",name().c_str(),pmt),Form("kernel for pmt %02i",pmt),nfreq,0,nfreq);
      for (int n=0; n < nfreq; n++){
	double r = k.real(n);
	double i = k.imag(n);
	hKernelFreqReal->SetBinContent(n+1,r);
	hKernelFreqImag->SetBinContent(n+1,i);
      }
      hKernelFreqReal->Write();
      hKernelFreqImag->Write();
      delete hKernelFreqReal;
      delete hKernelFreqImag;

    }// for all pmts
      
    return;
  }


  void DecoInterface::writeFilters(TFile* _fout){

    _fout->cd();

    for (int pmt=0; pmt < _pmt_num; pmt++){
      
      // kernel object for this pmt
      auto f = filter(pmt);

      // save time-domain kernel wf
      int size = f.size();
      TH1D* hFilterTime = new TH1D(Form("hFilter_%s_%02i",name().c_str(),pmt),Form("filter for pmt %02i",pmt),size,0,size);
      for (int n=0; n < size; n++)
	hFilterTime->SetBinContent(n+1,f[n]);
      hFilterTime->Write();
      delete hFilterTime;
    }// for all pmts
      
    return;
  }

  void DecoInterface::deconvolveKernels(TFile* _fout){

    _fout->cd();

    // vector of deconvolution normalization factors
    std::vector<double> deconvolutionNormalization;

    // create a TTree where to store area / amplitude changes
    double raw_area = 0;
    double raw_amp  = 0;
    double deco_area = 0;
    double deco_amp  = 0;
    double ratio_amp = 0;
    double ratio_area = 0;
    int pmt;
    TTree* tree = new TTree(Form("normalizationTree_%s",name().c_str()),"area / amplitude normalization change");;
    tree->Branch("pmt",&pmt,"pmt/I");
    tree->Branch("raw_area",&raw_area,"raw_area/D");
    tree->Branch("deco_area",&deco_area,"deco_area/D");
    tree->Branch("raw_amp",&raw_amp,"raw_amp/D");
    tree->Branch("deco_amp",&deco_amp,"deco_amp/D");
    tree->Branch("ratio_amp",&ratio_amp,"ratio_amp/D");
    tree->Branch("ratio_area",&ratio_area,"ratio_area/D");

    // use this function to deconvolve the kernel SPE response to obtain a measure of the signal's
    // magnitude amplification / suppression.
    for (pmt=0; pmt < _pmt_num; pmt++){
      auto k = kernel(pmt);
    // copy kernel into signal-like vector
    std::vector<double> signal(N(),0.);
    // calculate raw pulse area & amplitude

    for (size_t n=0; n < k.size(); n++){
      signal[n] = k[n];
      raw_area += k[n];
      if (k[n] > raw_amp) raw_amp = k[n];
    }
    auto processed_wf = ::signalana::DeconvolutionTool::Deconvolve(signal,pmt);
    // save the filtered signal to the tree
    int size = k.size();
    TH1D* hDeconvolvedKernel = new TH1D(Form("hDeconvolvedKernel_%s_%02i",name().c_str(),pmt),Form("deconvolved kernel for pmt %02i",pmt),size,0,size);
    // calculate deconvolved pulse area and amplitude

    for (int n=0; n < size; n++){
      hDeconvolvedKernel->SetBinContent(n+1,processed_wf[n]);
      deco_area += processed_wf[n];
      if (processed_wf[n] > deco_amp) deco_amp = processed_wf[n];
    }
    hDeconvolvedKernel->Write();
    delete hDeconvolvedKernel;

    ratio_area = deco_area/raw_area;
    ratio_amp  = deco_amp/raw_amp;
    deconvolutionNormalization.push_back(ratio_area);
    tree->Fill();

    }// for all PMTs

    LoadNormalizations(deconvolutionNormalization);

    tree->Write();

    return;
  }


}
#endif
