#include "stdio.h"
#include "stdlib.h"
#include "peachos.h"
#include <stdarg.h>

int putchar(int ch)
{
    peachos_putchar((char)ch);

    return 0;
}

#ifndef testing
int printf(const char *format, ...)
{
    va_list ap;
    char *sval;
    int ival;

    va_start(ap, format);
    for (const char *p = format; *p; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        switch (*++p)
        {
        case 'i':
            ival = va_arg(ap, int);
            print(itoa(ival));
            break;
        case 's':
            sval = va_arg(ap, char *);
            print(sval);
            break;
        default:
            putchar(*p);
            break;
        }
    }

    return 0;
}
#endif