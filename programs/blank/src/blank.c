#include "peachos.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    print("hello from blank\r");

    void *ptr = malloc(32);

    free(ptr);

    while (1)
    {
        char buf[] = {0, 0};
        buf[0] = getkey();

        if (buf[0])
        {
            print(buf);
        }
    }

    return 0;
}