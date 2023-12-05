#include <stdlib.h>

#include "cli.h"

const char *shiftarg(int *argc, char ***argv)
{
    if (*argc < 1)
    {
        return NULL;
    }

    const char *res = **argv;

    *argc -= 1;
    *argv += 1;

    return res;
}
