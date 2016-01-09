#ifndef SIGNALANA_DECONVOLUTIONTOOL_CXX
#define SIGNALANA_DECONVOLUTIONTOOL_CXX

#include "DeconvolutionTool.h"

namespace signalana{

  void DeconvolutionTool::PrepareFFTW3(const int& N){

    _N = N;
    
    _signal    = (double*)fftw_malloc(sizeof(double)*_N);
    _sigfft    = (complex*)fftw_malloc(sizeof(complex)*_N);
    _processed = (double*)fftw_malloc(sizeof(double)*_N);
    
    _forward_fft = fftw_plan_dft_r2c_1d(N,_signal,_sigfft,FFTW_MEASURE);

    _inverse_fft = fftw_plan_dft_c2r_1d(N,_sigfft,_processed,FFTW_MEASURE);

    return;
  }

  void DeconvolutionTool::ResetFFTW3(){
    
    fftw_free(_signal);
    fftw_free(_sigfft);
    fftw_free(_processed);
    
    fftw_destroy_plan(_forward_fft);
    fftw_destroy_plan(_inverse_fft);

    return;
  }

  void DeconvolutionTool::Deconvolve(const std::vector<double>& sig_v){

    if (sig_v.size() != _N){
      std::cout << "input vector size different than what this instance of the deconvolution tool is ready to handle" << std::endl;
      return;
    }

    std::copy(sig_v.begin(), sig_v.end(), _signal);

    fftw_execute(_forward_fft);

    return;
  }

}

#endif
