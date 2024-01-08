#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#include "io.h"
#include "see/da.h"
#include "see/io.h"

#define BUFSIZE 1024

static const char *find_delim(const char *start, size_t len, char delim)
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

static size_t signal_read_samples(const void *buf, size_t len, bool final, void *user_data)
{
    SignalBuffer *signal = user_data;

    const char *start = buf;
    double sample;
    size_t count = len;

    if (len == 0)
    {
        return 0;
    }

    if (final)
    {
        sscanf(start, "%lf", &sample);
        see_da_append(*signal, sample);
        return len;
    }

    const char *end = find_delim(start, count, '\n');

    while (end != NULL)
    {
        count -= (end - start) + 1;
        sscanf(start, "%lf", &sample);
        start = end + 1;
        end = find_delim(start, count, '\n');
        see_da_append(*signal, sample);
    }

    return len - count;
}

SignalBuffer signal_read_data(const char *file_path)
{
    SignalBuffer signal = {0};
    FILE *fd = fopen(file_path, "rb");

    if (fd == NULL)
    {
        fprintf(stderr, "Error reading signal data from file path %s: %s\n", file_path, strerror(errno));
        exit(1);
    }

    see_buffered_reader(fd, signal_read_samples, &signal);

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
