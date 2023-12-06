#include <stdio.h>

#include "../tools/cli.h"
#include "../tools/io.h"
#include "../tools/da.h"

#include "dft.h"

void fprint_usage(FILE *fd, const char *program)
{
    fprintf(fd, "Usage: %s <input_signal.dat> <rex.dat> <imx.dat>\n");
}

int main(int argc, char **argv)
{
    const char *program = shiftarg(&argc, &argv);

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
    SignalBuffer rex = {0};
    SignalBuffer imx = {0};

    signal_dft(input_signal.items, input_signal.count, &rex, &imx);

    signal_write_data(rex_path, rex.items, rex.count);
    signal_write_data(imx_path, imx.items, imx.count);
}