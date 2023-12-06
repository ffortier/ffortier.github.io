#ifndef DSP_DFT_H
#define DSP_DFT_H

#include <stdlib.h>

#include "../tools/io.h"

void signal_dft(const double *input_signal, size_t input_len, SignalBuffer *rex, SignalBuffer *imx);

#endif