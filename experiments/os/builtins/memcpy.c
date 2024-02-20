#include <stddef.h>

void *memcpy(void *destination, const void *source, size_t num)
{
    const char *s = source;
    char *d = destination;
    for (size_t i = 0; i < num; i++)
    {
        d[i] = s[i];
    }
    return destination;
}
