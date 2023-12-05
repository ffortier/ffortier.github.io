#include <string.h>

#include "convolution.h"

size_t signal_sizeof_convolution(size_t input_signal_len, size_t impulse_response_len)
{
    return input_signal_len + impulse_response_len;
}

size_t signal_convolution(double *output_signal, const double *input_signal, size_t input_signal_len, const double *impulse_response, size_t impulse_response_len)
{
    size_t output_signal_len = signal_sizeof_convolution(input_signal_len, impulse_response_len);

    memset(output_signal, 0, output_signal_len * sizeof(double));

    for (int i = 0; i < input_signal_len; i++)
    {
        for (int j = 0; j < impulse_response_len; j++)
        {
            output_signal[i + j] += input_signal[i] * impulse_response[j];
        }
    }

    return output_signal_len;
}