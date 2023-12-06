#ifndef DSP_CONVOLUTION_H
#define DSP_CONVOLUTION_H

#include <stdlib.h>
#include "../tools/io.h"

SignalBuffer signal_convolution_impulse_response(const double *input_signal, size_t input_signal_len, const double *impulse_response, size_t impulse_response_len);
SignalBuffer signal_convolution_running_sum(const double *input_signal, size_t input_signal_len);

#endif