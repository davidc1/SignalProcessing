#ifndef SIGNALANA_DECONVOLUTIONTOOL_CXX
#define SIGNALANA_DECONVOLUTIONTOOL_CXX

#include "DeconvolutionTool.h"

namespace signalana{

  DeconvolutionTool::DeconvolutionTool(){

    _padding = 0.;
    _debug = signalana::MessageLevel_t::kNORMAL;
  }

  DeconvolutionTool::DeconvolutionTool(const int& num_chan, const int& N, const double& sampling_freq){

    _N = N;
    _num_channels = num_chan;
    _sampling_freq = sampling_freq;

    _padding = 0.;

    _debug = signalana::MessageLevel_t::kNORMAL;
    for (int ch=0; ch < _num_channels; ch++){
      _kernel_v.push_back(Kernel(ch,_N,_sampling_freq));
      _filter_v.push_back(Filter(ch,_N,_sampling_freq));
    }

  }

  void DeconvolutionTool::PrepareFFTW3(){

    if (_debug == signalana::MessageLevel_t::kINFO)
      std::cout << "[function : PrepareFFTW3] -> about to allocate memory for arrays and prepare FFTW plans..." << std::endl;

    _signal    = (double*)fftw_malloc(sizeof(double)*_N);
    _sigfft    = (complex*)fftw_malloc(sizeof(complex)*_N);
    _processed = (double*)fftw_malloc(sizeof(double)*_N);
    
    _forward_fft = fftw_plan_dft_r2c_1d(_N,_signal,_sigfft,FFTW_MEASURE);

    _inverse_fft = fftw_plan_dft_c2r_1d(_N,_sigfft,_processed,FFTW_MEASURE);

    _normalization_v = std::vector<double>(_N,1.);

    if (_debug == signalana::MessageLevel_t::kINFO)
      std::cout << "[function : PrepareFFTW3] -> ...done" << std::endl;

    return;
  }

  void DeconvolutionTool::ResetFFTW3(){

    if (_debug == signalana::MessageLevel_t::kINFO)
      std::cout << "[function : ResetFFTW3] -> clearing FFTW3 memory/plans..." << std::endl;
    
    fftw_free(_signal);
    fftw_free(_sigfft);
    fftw_free(_processed);
    
    fftw_destroy_plan(_forward_fft);
    fftw_destroy_plan(_inverse_fft);

    if (_debug == signalana::MessageLevel_t::kINFO)
      std::cout << "[function : ResetFFTW3] -> ...done" << std::endl;

    return;
  }

  double* DeconvolutionTool::Deconvolve(const std::vector<double>& sig_v, const size_t& ch){

    if ((int)sig_v.size() != _N){
      throw SignalAnaException("input vector size different than what this instance of the deconvolution tool is ready to handle");
      return _processed;
    }

    std::copy(sig_v.begin(), sig_v.end(), _signal);

    // obtain the FT of this signal waveform
    fftw_execute(_forward_fft);

    // now apply complex division between the FFTs of the signal and kernel
    // _sigfft contains the signal's fft
    for (int n=0; n < _N/2+1; n++){
      // numerator   = a + ib
      // denominator = c + id
      double c = _kernel_v[ch].real(n)+_padding; // 5 is for padding
      double d = _kernel_v[ch].imag(n);
      double a = _sigfft[n][0];
      double b = _sigfft[n][1];

      double den = c*c+d*d;

      if (den != 0){
	double e = (a*c+b*d)/den;
	double f = (b*c-a*d)/den;
	_sigfft[n][0] = e* _filter_v[22][n];
	_sigfft[n][1] = f* _filter_v[22][n];
      }
      
    }// for all ticks -> applying division
    
    // now apply the inverse fft to obtain the deconvolved signal
    fftw_execute(_inverse_fft);

    // now scale by the loss in power
    for (int n=0; n < _N; n++){
      _processed[n] /= (_normalization_v[ch]*_N);
    }

    return _processed;
  }

  void DeconvolutionTool::LoadKernel(const int& chnum, const std::vector<double>& kernel_vals){

    if (chnum >= _num_channels)
      throw SignalAnaException("ERROR: trying to setup a kernel for channel which is out of range");

    if (chnum >= (int)_kernel_v.size())
      throw SignalAnaException("ERROR: trying to setup a kernel for channel which is out of range");

    // add values to kernel
    _kernel_v[chnum].clear();
    for (size_t i=0; i < kernel_vals.size(); i++)
      _kernel_v[chnum].push_back(kernel_vals[i]);
    
    // pad the kernel and compute FT
    // 1) find maximum tick
    if (_debug == signalana::MessageLevel_t::kDEBUG)
      std::cout << "[function : LoadKernels] -> find max tick of kernel" << std::endl;
    size_t max_tick = 0;
    double max_val  = 0;
    for (size_t i=0; i < _kernel_v[chnum].size(); i++)
      if (_kernel_v[chnum][i] > max_val) { max_val = _kernel_v[chnum][i]; max_tick = i; }
    // 2) fill the signal vector with the padded kernel
    if (_debug == signalana::MessageLevel_t::kDEBUG)
      std::cout << "[function : LoadKernels] -> pad kernel" << std::endl;
    for (int n=0; n < _N; n++){
      // if we are still in the falling-edge of the kernel -> add it to front of _signal
      if ( (max_tick+n) < _kernel_v[chnum].size() ) { _signal[n] = _kernel_v[chnum][max_tick+n]; }
      // if we are in the rising edge of the kernel -> add to the end of _signal
      else if (n <= (int)max_tick) { _signal[n] = _kernel_v[chnum][n]; }
      // otherwise, pad w/ zeros
      else { _signal[n] = 0.; }
    }// fill the _signal vector
    // 3) execute fft to find the freq. domain kernel response
    if (_debug == signalana::MessageLevel_t::kDEBUG)
      std::cout << "[function : LoadKernels] -> execute FFT on kernel time-domain array" << std::endl;
    fftw_execute(_forward_fft);
    // 4) save the fft for the kernel
    if (_debug == signalana::MessageLevel_t::kDEBUG)
      std::cout << "[function : LoadKernels] -> set the kernel's freq-domain array" << std::endl;
    _kernel_v[chnum].setFFT(_sigfft,_N);
    
    return;
  }
  
  
  void DeconvolutionTool::LoadKernels(const std::vector<std::vector<double> >& kernel_v){

    for (size_t i=0; i < kernel_v.size(); i++){
      if (_debug == signalana::MessageLevel_t::kDEBUG)
	std::cout << "[function : LoadKernels] -> begin loading kernel for channel " << i << std::endl;
      Kernel k(kernel_v[i]);
      // set the kernel's channel number
      k.setChannel(i);
      // pad the kernel and compute FT
      // 1) find maximum tick
      if (_debug == signalana::MessageLevel_t::kDEBUG)
	std::cout << "[function : LoadKernels] -> find max tick of kernel" << std::endl;
      size_t max_tick = 0;
      double max_val  = 0;
      for (size_t i=0; i < k.size(); i++)
	if (k[i] > max_val) { max_val = k[i]; max_tick = i; }
      // 2) fill the signal vector with the padded kernel
      if (_debug == signalana::MessageLevel_t::kDEBUG)
	std::cout << "[function : LoadKernels] -> pad kernel" << std::endl;
      for (int n=0; n < _N; n++){
	// if we are still in the falling-edge of the kernel -> add it to front of _signal
	if ( (max_tick+n) < k.size() ) { _signal[n] = k[max_tick+n]; }
	// if we are in the rising edge of the kernel -> add to the end of _signal
	else if (n <= (int)max_tick) { _signal[n] = k[n]; }
	// otherwise, pad w/ zeros
	else { _signal[n] = 0.; }
      }// fill the _signal vector
      // 3) execute fft to find the freq. domain kernel response
      if (_debug == signalana::MessageLevel_t::kDEBUG)
	std::cout << "[function : LoadKernels] -> execute FFT on kernel time-domain array" << std::endl;
      fftw_execute(_forward_fft);
      // 4) save the fft for the kernel
      if (_debug == signalana::MessageLevel_t::kDEBUG)
	std::cout << "[function : LoadKernels] -> set the kernel's freq-domain array" << std::endl;
      k.setFFT(_sigfft,_N);
      

      _kernel_v.push_back(k);
    }
    
    return;
  }

  void DeconvolutionTool::LoadFilter(const int& chnum){

    if (chnum >= _num_channels)
      throw SignalAnaException("ERROR: trying to setup a filter for channel which is out of range");

    if (chnum >= (int)_filter_v.size())
      throw SignalAnaException("ERROR: trying to setup a filter for channel which is out of range");

    _filter_v[chnum] = Filter(chnum,_N,_sampling_freq);
    
    return;
  }


  void DeconvolutionTool::LoadFilters(const std::vector<std::vector<double> >& filter_v){

    for (size_t i=0; i < filter_v.size(); i++){
      Filter f(filter_v[i]);
      f.setChannel(i);
      _filter_v.push_back(f);
    }
    
    return;
  }

  const signalana::Kernel& DeconvolutionTool::kernel(const int& n){

    if (n >= (int)_kernel_v.size()){
      throw SignalAnaException("ERROR: asking for a kernel channel that is not stored.");
      return signalana::Kernel();
    }

    return _kernel_v[n];
  }

  const signalana::Filter& DeconvolutionTool::filter(const int& n){

    if (n >= (int)_filter_v.size()){
      throw SignalAnaException("ERROR: asking for a filter channel that is not stored.");
      return signalana::Filter();
    }

    return _filter_v[n];
  }




}

#endif
