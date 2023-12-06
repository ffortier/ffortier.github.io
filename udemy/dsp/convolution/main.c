#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "convolution.h"
#include "../tools/cli.h"
#include "../tools/io.h"
#include "../tools/da.h"

static void fprint_usage(FILE *fd, const char *program)
{
    fprintf(fd, "Usage: %s <subcommand>\n", program);
    fprintf(fd, "Subcommands:\n");
    fprintf(fd, "\timpulse_response <input_signal.dat> <impulse_response.dat>\n");
    fprintf(fd, "\trunning_sum <input_signal.dat>\n");
}

static void fprint_signal(FILE *fd, const double *samples, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        fprintf(fd, "%lf\n", samples[i]);
    }
}

static int impulse_response(int argc, char **argv, const char *program)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s impulse_response <input_signal.dat> <impulse_response.dat>\n", program);
        fprintf(stderr, "Error: Missing arguments\n");
        return 1;
    }

    const char *input_signal_path = shiftarg(&argc, &argv);
    const char *impulse_response_path = shiftarg(&argc, &argv);

    SignalBuffer input_signal = signal_read_data(input_signal_path);
    SignalBuffer impulse_response = signal_read_data(impulse_response_path);
    SignalBuffer output_signal = signal_convolution_impulse_response(input_signal.items, input_signal.count, impulse_response.items, impulse_response.count);

    fprint_signal(stdout, output_signal.items, output_signal.count);

    da_free(input_signal);
    da_free(impulse_response);
    da_free(output_signal);

    return 0;
}

static int running_sum(int argc, char **argv, const char *program)
{
    if (argc != 1)
    {
        fprintf(stderr, "Usage: %s running_sum <input_signal.dat>\n", program);
        fprintf(stderr, "Error: Missing arguments\n");
        return 1;
    }

    const char *input_signal_path = shiftarg(&argc, &argv);

    SignalBuffer input_signal = signal_read_data(input_signal_path);
    SignalBuffer output_signal = signal_convolution_running_sum(input_signal.items, input_signal.count);

    fprint_signal(stdout, output_signal.items, output_signal.count);

    da_free(input_signal);
    da_free(output_signal);

    return 0;
}

int main(int argc, char **argv)
{
    const char *program = shiftarg(&argc, &argv);

    if (argc < 1)
    {
        fprint_usage(stderr, program);
        fprintf(stderr, "Error: Missing subcommand\n");
        return 1;
    }

    const char *subcommand = shiftarg(&argc, &argv);

    if (strcmp("impulse_response", subcommand) == 0)
    {
        return impulse_response(argc, argv, program);
    }

    if (strcmp("running_sum", subcommand) == 0)
    {
        return running_sum(argc, argv, program);
    }

    fprint_usage(stderr, program);
    fprintf(stderr, "Error: Unknown subcommand %s\n", subcommand);

    return 1;
}