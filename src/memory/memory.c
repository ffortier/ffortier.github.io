#include "memory.h"

void *memset(void *ptr, int c, size_t size)
{
    char *c_ptr = (char *)ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char)c;
    }
    return ptr;
}

int memcmp(const void *ptr1, const void *ptr2, size_t len)
{
    const char *c1 = ptr1;
    const char *c2 = ptr2;
    for (size_t i = 0; i < len; i++)
    {
        if (c1[i] < c2[i])
        {
            return -i;
        }
        else if (c1[i] > c2[i])
        {
            return i;
        }
    }
    return 0;
}

void *memcpy(void *dest, const void *src, size_t len)
{
    char *d = dest;
    const char *s = src;
    while (len--)
    {
        *d++ = *s++;
    }
    return dest;
}