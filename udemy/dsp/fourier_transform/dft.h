#ifndef DSP_DFT_H
#define DSP_DFT_H

#include <stdlib.h>

#include "../tools/io.h"

typedef struct
{
    SignalBuffer rex;
    SignalBuffer imx;
} FreqBuffer;

FreqBuffer signal_dft(const double *input_signal, size_t input_len);
SignalBuffer signal_idft(const double *rex, const double *imx, size_t freq_len);
SignalBuffer signal_dft_magnetude(const double *rex, const double *imx, size_t freq_len);

#endif