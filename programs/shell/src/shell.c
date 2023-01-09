#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "peachos.h"

int main(int argc, char **argv)
{
    print("PeachOS v1.0.0\n");
    while (1)
    {
        print("> ");
        char buf[1024];
        buf[0] = '0';
        buf[1] = ':';
        peachos_terminal_readline(buf + 2, sizeof(buf) - 3, true);
        print("\n");
        peachos_process_load_start(buf);
    }
    return 0;
}