#ifndef SEE_MEM_H
#define SEE_MEM_H

#ifndef SEE_CUSTOM_ALLOCATORS
#include <stdlib.h>
#include <string.h>
#define see_malloc(size) malloc(size)
#define see_realloc(ptr, size) realloc(ptr, size)
#define see_free(ptr) free(ptr)
#define see_calloc(count, size) calloc(count, size)
#define see_memset(ptr, c, len) memset(ptr, c, len)
#endif

#endif