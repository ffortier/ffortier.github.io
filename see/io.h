#ifndef SEE_IO
#define SEE_IO

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef size_t (*read_callback)(const void *buffer, size_t len, bool end, void *user_data);

size_t see_buffered_reader(FILE *fd, read_callback cb, void *user_data);

#endif