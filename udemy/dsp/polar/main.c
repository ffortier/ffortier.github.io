#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "see/cli.h"
#include "see/da.h"

#include "../tools/io.h"

const double PI = 3.14159265358979;

typedef struct
{
    SignalBuffer phase_signal;
    SignalBuffer magnetude_signal;
} PolarBuffer;

void free_polar_buffer(PolarBuffer *polar_buffer)
{
    see_da_free(polar_buffer->magnetude_signal);
    see_da_free(polar_buffer->phase_signal);
}

void print_usage(FILE *out, const char *program)
{
    fprintf(out, "Usage: %s <subcommand>\n", program);
    fprintf(out, "Subcommands:\n");
    fprintf(out, "\tpolar <mag_out.dat> <phase_out.dat> <rex_in.dat> <imx_in.dat>\n");
}

PolarBuffer polar_from_rect(const double *rex, const double *imx, size_t count)
{
    PolarBuffer buf = {0};

    see_da_init(buf.magnetude_signal, count);
    see_da_init(buf.phase_signal, count);

    for (size_t k = 0; k < count; k++)
    {
        buf.magnetude_signal.items[k] = sqrt(pow(rex[k], 2) + pow(imx[k], 2));

        if (rex[0] == 0)
        {
            buf.phase_signal.items[k] = atan(imx[k] / pow(10.0, -20.0));
        }

        if (rex[k] < 0 && imx[k] < 0)
        {
            buf.phase_signal.items[k] -= PI;
        }

        if (rex[k] < 0 && imx[k] >= 0)
        {
            buf.phase_signal.items[k] += PI;
        }
    }

    buf.magnetude_signal.count = count;
    buf.phase_signal.count = count;

    return buf;
}

int polar_command(int argc, char **argv, const char *program)
{
    if (argc != 4)
    {
        print_usage(stderr, program);
        fprintf(stderr, "Error: missing arguments\n");
        return 1;
    }

    const char *mag_out_path = shiftarg(&argc, &argv);
    const char *phase_out_path = shiftarg(&argc, &argv);
    const char *rex_in_path = shiftarg(&argc, &argv);
    const char *imx_in_path = shiftarg(&argc, &argv);

    SignalBuffer rex_buffer = signal_read_data(rex_in_path);
    SignalBuffer imx_buffer = signal_read_data(imx_in_path);

    assert(rex_buffer.count == imx_buffer.count && "REX and IMX must have the same size");

    PolarBuffer polar_buffer = polar_from_rect(rex_buffer.items, imx_buffer.items, rex_buffer.count);

    signal_write_data(mag_out_path, polar_buffer.magnetude_signal.items, polar_buffer.magnetude_signal.count);
    signal_write_data(phase_out_path, polar_buffer.phase_signal.items, polar_buffer.phase_signal.count);

    see_da_free(rex_buffer);
    see_da_free(imx_buffer);
    free_polar_buffer(&polar_buffer);

    return 0;
}

int main(int argc, char **argv)
{
    const char *program = shiftarg(&argc, &argv);

    if (argc < 1)
    {
        print_usage(stderr, program);
        fprintf(stderr, "Error: Missing subcommand\n");
        return 1;
    }

    const char *subcommand = shiftarg(&argc, &argv);

    if (strcmp(subcommand, "polar") == 0)
    {
        return polar_command(argc, argv, program);
    }

    return 1;
}