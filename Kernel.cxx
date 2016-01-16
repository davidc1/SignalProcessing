#ifndef SIGNALANA_KERNEL_CXX
#define SIGNALANA_KERNEL_CXX

#include "Kernel.h"

namespace signalana{

  Kernel::Kernel(const std::vector<double>& time_v)
    : std::vector<double>(time_v)
  {}

  Kernel::Kernel(const int& chnum, const int& samples, const double& sampling_freq)
    : Channel(chnum,samples,sampling_freq)
  {}

  void Kernel::Clear(){
    
    this->clear();

    return;
  }

  void Kernel::setFFT(const fftw_complex *fft, const int& N){

    _freq_space = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    
    for (int i=0; i < N; i++){
      for(int j=0; j < 2; j++){
	_freq_space[i][j] = fft[i][j];
	//std::cout << "kernel element [i,j] -> [" << i << ", " << j << "] = " << _freq_space[i][j] << std::endl; 
      }
    }

    _samples = N;
    
    return;
  }

  double Kernel::real(const int& n){
    
    if (n >= _samples){
      std::cout << "Error: accessing element not stored in Kernel's FFT" << std::endl;
      return -1;
    }

    return _freq_space[n][0];
  }

  double Kernel::imag(const int& n){
    
    if (n >= _samples){
      std::cout << "Error: accessing element not stored in Kernel's FFT" << std::endl;
      return -1;
    }

    return _freq_space[n][1];
  }
    

}

#endif
