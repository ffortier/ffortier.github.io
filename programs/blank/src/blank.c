#include "../../stdlib/src/peachos.h"
#include "../../stdlib/src/stdlib.h"
#include "../../stdlib/src/stdio.h"
#include "../../stdlib/src/string.h"

int main(int argc, char **argv)
{
    char words[] = "hello how are you";

    char *token = strtok(words, " ");

    while (token)
    {
        printf("%s\n", token);
        token = strtok(words, " ");
    }

    printf("hello from %s %i\r", "blank", 42);

    while (1)
        ;
    return 0;
}