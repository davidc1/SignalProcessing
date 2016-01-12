#ifndef SIGNALANA_KERNEL_CXX
#define SIGNALANA_KERNEL_CXX

#include "Kernel.h"

namespace signalana{

  Kernel::Kernel(const std::vector<double>& time_v)
    : std::vector<double>(time_v)
  {}

  void Kernel::Clear(){
    
    this->clear();
    _channel = -1;

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

    _N = N;
    
    return;
  }

  const double Kernel::real(const int& n){
    
    if (n >= _N){
      std::cout << "Error: accessing element not stored in Kernel's FFT" << std::endl;
      return -1;
    }

    return _freq_space[n][0];
  }

  const double Kernel::imag(const int& n){
    
    if (n >= _N){
      std::cout << "Error: accessing element not stored in Kernel's FFT" << std::endl;
      return -1;
    }

    return _freq_space[n][1];
  }
    

}

#endif
