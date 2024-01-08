#include <string.h>

#include "io.h"

#define BUFSIZE 1024

size_t see_buffered_reader(FILE *fd, read_callback cb, void *user_data)
{
    static uint8_t buf[BUFSIZE];

    size_t count = fread(buf, 1, BUFSIZE, fd);
    size_t total = 0;

    while (count > 0)
    {
        total += count;

        size_t consumed = cb(&buf[0], count, false, user_data);
        size_t offset = count - consumed;

        if (count != BUFSIZE) // EOF
        {
            (void)cb(&buf[consumed], offset, true, user_data);

            return total;
        }

        if (offset > 0)
        {
            memmove(&buf[0], &buf[consumed], offset);
        }

        count = fread(&buf[offset], 1, BUFSIZE - offset, fd) + offset;
    }

    return total;
}