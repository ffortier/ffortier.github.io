#include "peachos.h"
#include "stdlib.h"
#include "stdio.h"

int main(int argc, char **argv)
{
    printf("hello from %s %i\r", "blank", 42);

    char buf[1024];

    peachos_terminal_readline(buf, 1024, false);

    printf("read '%s'", buf);

    while (1)
        ;
    return 0;
}