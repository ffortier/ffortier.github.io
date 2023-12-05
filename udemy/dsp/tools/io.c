#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "io.h"
#include "da.h"

#define BUFSIZE 1024

static char *find_delim(char *start, size_t len, char delim)
{
    for (int i = 0; i < len; i++)
    {
        if (start[i] == delim)
        {
            return &start[i];
        }
    }
    return NULL;
}

SignalBuffer signal_read_data(const char *file_path)
{
    SignalBuffer signal = {0};

    static char buf[BUFSIZE];

    FILE *fd = fopen(file_path, "rb");

    if (fd == NULL)
    {
        fprintf(stderr, "Error reading signal data from file path %s: %s\n", file_path, strerror(errno));
        exit(1);
    }

    size_t count = fread(buf, sizeof(char), BUFSIZE, fd);
    double sample;

    while (count > 0)
    {
        char *start = buf;
        char *end = find_delim(buf, count, '\n');

        while (end != NULL)
        {
            count -= (end - start) + 1;
            sscanf(start, "%lf", &sample);
            start = end + 1;
            end = find_delim(start, count, '\n');
            da_append(signal, sample);
        }

        if (count)
        {
            if (end == NULL && (start - buf) + count != BUFSIZE) // EOF
            {
                sscanf(start, "%lf", &sample);
                da_append(signal, sample);
                count = 0;
            }

            memmove(buf, start, count * sizeof(&buf[0]));
        }

        count = fread(&buf[count], sizeof(char), BUFSIZE - count, fd) + count;
    }

    fclose(fd);

    return signal;
}

void signal_write_data(const char *file_path, double *samples, size_t count)
{
    FILE *fd = fopen(file_path, "wb");

    if (fd == NULL)
    {
        fprintf(stderr, "Error writing signal data from file path %s: %s\n", file_path, strerror(errno));
        exit(1);
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fd, "%lf\n", samples[i]);
    }

    fclose(fd);
}
