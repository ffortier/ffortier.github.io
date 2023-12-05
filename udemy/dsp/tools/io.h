#ifndef DSP_IO_H
#define DSP_IO_H

#include <stdlib.h>

typedef struct
{
    double *items;
    size_t count;
    size_t capacity;
} SignalBuffer;

SignalBuffer signal_read_data(const char *file_path);

void signal_write_data(const char *file_path, double *samples, size_t count);

#endif