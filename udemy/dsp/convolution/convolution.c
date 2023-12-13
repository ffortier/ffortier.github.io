#include <string.h>

#include "convolution.h"
#include "see/da.h"

SignalBuffer signal_convolution_impulse_response(const double *input_signal, size_t input_signal_len, const double *impulse_response, size_t impulse_response_len)
{
    SignalBuffer output_signal = {0};

    see_da_init(output_signal, input_signal_len + impulse_response_len);

    for (int i = 0; i < input_signal_len; i++)
    {
        for (int j = 0; j < impulse_response_len; j++)
        {
            output_signal.items[i + j] += input_signal[i] * impulse_response[j];
        }
    }

    output_signal.count = input_signal_len + impulse_response_len;

    return output_signal;
}

SignalBuffer signal_convolution_running_sum(const double *input_signal, size_t input_signal_len)
{
    SignalBuffer output_signal = {0};

    see_da_init(output_signal, input_signal_len);

    output_signal.items[0] = input_signal[0];

    for (int i = 1; i < input_signal_len; i++)
    {
        output_signal.items[i] = output_signal.items[i - 1] + input_signal[i];
    }

    output_signal.count = input_signal_len;

    return output_signal;
}
