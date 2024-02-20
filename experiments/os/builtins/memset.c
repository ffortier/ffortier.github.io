#include <stddef.h>

void *memset(void *ptr, char value, size_t num)
{
    char *d = ptr;
    for (size_t i = 0; i < num; i++)
    {
        d[i] = value;
    }
    return ptr;
}
