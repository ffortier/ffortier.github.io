#include <math.h>

#include "dft.h"
#include "../tools/da.h"

const double PI = 3.14159265359;

void signal_dft(const double *input_signal, size_t input_len, SignalBuffer *rex, SignalBuffer *imx)
{
    da_init(*rex, input_len / 2);
    da_init(*imx, input_len / 2);

    for (int k = 0; k < input_len / 2; k++)
    {
        for (int i = 0; i < input_len; i++)
        {
            rex->items[k] += input_signal[i] * cos(2 * PI * k * i / input_len);
            imx->items[k] -= input_signal[i] * sin(2 * PI * k * i / input_len);
        }
    }

    rex->count = input_len / 2;
    imx->count = input_len / 2;
}