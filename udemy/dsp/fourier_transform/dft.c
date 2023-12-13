#include <math.h>

#include "dft.h"
#include "see/da.h"

const double PI = 3.14159265359;

FreqBuffer signal_dft(const double *input_signal, size_t input_len)
{
    SignalBuffer rex = {0};
    SignalBuffer imx = {0};

    double n = (double)input_len;

    see_da_init(rex, input_len / 2);
    see_da_init(imx, input_len / 2);

    for (int k = 0; k < input_len / 2; k++)
    {
        for (int i = 0; i < input_len; i++)
        {
            rex.items[k] += input_signal[i] * cos(2 * PI * k * i / n);
            imx.items[k] -= input_signal[i] * sin(2 * PI * k * i / n);
        }
    }

    rex.count = n / 2;
    imx.count = n / 2;

    return (FreqBuffer){.rex = rex, .imx = imx};
}

SignalBuffer signal_idft(const double *rex, const double *imx, size_t freq_len)
{
    SignalBuffer output_signal = {0};
    SignalBuffer rex_buffer = {0};
    SignalBuffer imx_buffer = {0};

    double n = freq_len * 2.0;

    see_da_init(output_signal, freq_len * 2);
    see_da_extend(rex_buffer, rex, freq_len);
    see_da_extend(imx_buffer, imx, freq_len);

    for (int k = 0; k < freq_len; k++)
    {
        rex_buffer.items[k] /= n;
        imx_buffer.items[k] /= -n;
    }

    for (int k = 0; k < freq_len; k++)
    {
        for (int i = 0; i < freq_len * 2; i++)
        {
            output_signal.items[i] += rex_buffer.items[k] * cos(2 * PI * k * i / n) + imx_buffer.items[k] * sin(2 * PI * k * i / n);
        }
    }

    output_signal.count = freq_len * 2;

    return output_signal;
}

SignalBuffer signal_dft_magnetude(const double *rex, const double *imx, size_t freq_len)
{
    SignalBuffer mag = {0};

    see_da_init(mag, freq_len);

    for (int k = 0; k < freq_len; k++)
    {
        mag.items[k] = sqrt(pow(rex[k], 2) + pow(imx[k], 2));
    }

    mag.count = freq_len;

    return mag;
}