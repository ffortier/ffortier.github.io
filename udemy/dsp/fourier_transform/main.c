#include <stdio.h>
#include <assert.h>

#include "../tools/cli.h"
#include "../tools/io.h"
#include "../tools/da.h"

#include "dft.h"

void fprint_usage(FILE *fd, const char *program)
{
    fprintf(fd, "Usage: %s <subcommand>\n", program);
    fprintf(fd, "Subcommands:\n");
    fprintf(fd, "\tdft <input_signal.dat> <rex.dat> <imx.dat>\n");
    fprintf(fd, "\tidft <output_signal.dat> <rex.dat> <imx.dat>\n");
}

int dft_command(int argc, char **argv, const char *program)
{
    if (argc != 3)
    {
        fprint_usage(stderr, program);
        fprintf(stderr, "Error: Missing arguments\n");
        return 1;
    }

    const char *input_signal_path = shiftarg(&argc, &argv);
    const char *rex_path = shiftarg(&argc, &argv);
    const char *imx_path = shiftarg(&argc, &argv);

    SignalBuffer input_signal = signal_read_data(input_signal_path);
    FreqBuffer freq = signal_dft(input_signal.items, input_signal.count);

    assert(freq.rex.count == freq.imx.count && "Expected same number of frequencies for real and imaginary components");

    signal_write_data(rex_path, freq.rex.items, freq.rex.count);
    signal_write_data(imx_path, freq.imx.items, freq.imx.count);

    return 0;
}

int idft_command(int argc, char **argv, const char *program)
{
    if (argc != 3)
    {
        fprint_usage(stderr, program);
        fprintf(stderr, "Error: Missing arguments\n");
        return 1;
    }

    const char *output_signal_path = shiftarg(&argc, &argv);
    const char *rex_path = shiftarg(&argc, &argv);
    const char *imx_path = shiftarg(&argc, &argv);

    SignalBuffer rex = signal_read_data(rex_path);
    SignalBuffer imx = signal_read_data(imx_path);

    assert(rex.count == imx.count && "Expected same number of frequencies for real and imaginary components");

    SignalBuffer output_signal = signal_idft(rex.items, imx.items, rex.count);

    signal_write_data(output_signal_path, output_signal.items, output_signal.count);

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

    if (strcmp(subcommand, "dft") == 0)
    {
        return dft_command(argc, argv, program);
    }

    if (strcmp(subcommand, "idft") == 0)
    {
        return idft_command(argc, argv, program);
    }

    fprint_usage(stderr, program);
    fprintf(stderr, "Error: Invalid subcommand %s\n", subcommand);
    return 1;
}