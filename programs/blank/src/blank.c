#include "../../stdlib/src/peachos.h"
#include "../../stdlib/src/stdlib.h"
#include "../../stdlib/src/stdio.h"
#include "../../stdlib/src/string.h"

int main(int argc, char **argv)
{
    char str[] = "hello world";

    __auto_type current = peachos_parse_command(str, sizeof(str));

    while (current)
    {
        printf("%s\n", current->argument);
        current = current->next;
    }

    while (1)
        ;
    return 0;
}