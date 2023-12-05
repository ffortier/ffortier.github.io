#ifndef DSP_CONVOLUTION_H
#define DSP_CONVOLUTION_H
#include <stdlib.h>

size_t signal_sizeof_convolution(size_t input_signal_len, size_t impulse_response_len);
size_t signal_convolution(double *output_signal, const double *input_signal, size_t input_signal_len, const double *impulse_response, size_t impulse_response_len);
#endif