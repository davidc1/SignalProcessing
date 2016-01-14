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
    for (int n=0; n < N(); n++)
      wfout.push_back((short)processed_wf[n]);
    wfout.SetChannelNumber(wf.ChannelNumber());
    wfout.SetTimeStamp(wf.TimeStamp());

    return wfout;
  }

  void DecoInterface::writeKernels(TFile* _fout){

    _fout->cd();

    for (int pmt=0; pmt < 32; pmt++){
      
      // kernel object for this pmt
      auto k = kernel(pmt);

      // save time-domain kernel wf
      int size = k.size();
      TH1D* hKernelTime = new TH1D(Form("hKernelTime_%02i",pmt),Form("kernel for pmt %02i",pmt),size,0,size);
      for (int n=0; n < size; n++)
	hKernelTime->SetBinContent(n+1,k[n]);
      hKernelTime->Write();
      delete hKernelTime;

      // save freq-domain kernel info
      int nfreq = N()/2+1;
      TH1D* hKernelFreqReal = new TH1D(Form("hKernelFreqReal_%02i",pmt),Form("kernel for pmt %02i",pmt),nfreq,0,nfreq);
      TH1D* hKernelFreqImag = new TH1D(Form("hKernelFreqImag_%02i",pmt),Form("kernel for pmt %02i",pmt),nfreq,0,nfreq);
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

    for (int pmt=0; pmt < 32; pmt++){
      
      // kernel object for this pmt
      auto f = filter(pmt);

      // save time-domain kernel wf
      int size = f.size();
      TH1D* hFilterTime = new TH1D(Form("hFilter_%02i",pmt),Form("filter for pmt %02i",pmt),size,0,size);
      for (int n=0; n < size; n++)
	hFilterTime->SetBinContent(n+1,f[n]);
      hFilterTime->Write();
      delete hFilterTime;
    }// for all pmts
      
    return;
  }


}
#endif
