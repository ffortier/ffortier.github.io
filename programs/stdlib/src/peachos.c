#include "peachos.h"

int peachos_getkeyblock()
{
    int val;

    do
    {
        val = peachos_getkey();
    } while (!val);

    return val;
}

void peachos_terminal_readline(char *out, int max, bool output_while_typing)
{
    for (int i = 0; i < max - 1; i++)
    {
        char c = peachos_getkeyblock();

        if (c == 13)
        {
            break;
        }

        if (output_while_typing)
        {
            peachos_putchar(c);
        }

        if (c == 8 && i > 0)
        {
            *--out = 0;
            i -= 2;
            continue;
        }

        *out++ = c;
    }

    *out = 0;
}