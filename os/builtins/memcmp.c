#include <stddef.h>
#include <stdint.h>

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    const uint8_t *a = ptr1;
    const uint8_t *b = ptr2;

    for (size_t i = 0; i < num; i++)
    {
        if (a[i] < b[i])
        {
            return -i;
        }

        if (a[i] > b[i])
        {
            return i;
        }
    }

    return 0;
}