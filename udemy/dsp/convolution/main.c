#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "convolution.h"
#include "../tools/cli.h"
#include "../tools/io.h"
#include "../tools/da.h"

int main(int argc, char **argv)
{
    const char *program = shiftarg(&argc, &argv);

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_signal.dat> <impulse_response.dat>\n", program);
        fprintf(stderr, "Error: Missing arguments");
        return 1;
    }

    const char *input_signal_path = shiftarg(&argc, &argv);
    const char *impulse_response_path = shiftarg(&argc, &argv);

    SignalBuffer input_signal = signal_read_data(input_signal_path);
    SignalBuffer impulse_response = signal_read_data(impulse_response_path);
    SignalBuffer output_signal = {0};

    da_init(output_signal, signal_sizeof_convolution(input_signal.count, impulse_response.count));

    output_signal.count = signal_convolution(output_signal.items, input_signal.items, input_signal.count, impulse_response.items, impulse_response.count);

    for (size_t i = 0; i < output_signal.count; i++)
    {
        printf("%lf\n", output_signal.items[i]);
    }

    return 0;
}