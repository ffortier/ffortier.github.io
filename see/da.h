#ifndef SEE_DA_H
#define SEE_DA_H

#include <assert.h>
#include "see/mem.h"

/**
 * typedef struct {
 *     T* items;
 *     size_t count;
 *     size_t len;
 * } DynamicArray<T>;
 */

/**
 * Extends a dynamic array `da` with an array `data` of size `len`
 * see_da_extend<T>(DynamicArray<T> &da, T* data, size_t len)
 */
#define see_da_extend(da, data, len)                                                      \
    do                                                                                    \
    {                                                                                     \
        while ((da).count + (len) > (da).capacity)                                        \
        {                                                                                 \
            (da).capacity = (da).capacity == 0 ? 100 : (da).capacity * 2;                 \
            (da).items = see_realloc((da).items, sizeof(&(da).items[0]) * (da).capacity); \
            assert((da).items && "Could not realloc memory");                             \
        }                                                                                 \
        memcpy(&(da).items[(da).count], data, sizeof(&(da).items[0]) * (len));            \
        (da).count += len;                                                                \
    } while (0)

/**
 * Appends a single item `item` to a dynamic array `da`
 * see_da_append<T>(DynamicArray<T> &da, T data)
 */
#define see_da_append(da, item)                                                           \
    do                                                                                    \
    {                                                                                     \
        while ((da).count + 1 > (da).capacity)                                            \
        {                                                                                 \
            (da).capacity = (da).capacity == 0 ? 100 : (da).capacity * 2;                 \
            (da).items = see_realloc((da).items, sizeof(&(da).items[0]) * (da).capacity); \
            assert((da).items && "Could not realloc memory");                             \
        }                                                                                 \
        (da).items[(da).count++] = item;                                                  \
    } while (0)

/**
 * Ensures the dynamic has enough capacity
 * see_da_init<T>(DynamicArray<T> &da, T data)
 */
#define see_da_ensure_capacity(da, min_capacity)                                          \
    do                                                                                    \
    {                                                                                     \
        if ((da).capacity < min_capacity)                                                 \
        {                                                                                 \
            (da).capacity = min_capacity;                                                 \
            (da).items = see_realloc((da).items, sizeof(&(da).items[0]) * (da).capacity); \
            assert((da).items && "Could not realloc memory");                             \
        }                                                                                 \
    } while (0)

/**
 * Ensures the dynamic has enough capacity and zero initialize the buffer
 * see_da_init<T>(DynamicArray<T> &da, T data)
 */
#define see_da_init(da, initial_capacity)                                  \
    do                                                                     \
    {                                                                      \
        see_da_ensure_capacity(da, initial_capacity);                      \
        (da).count = 0;                                                    \
        see_memset((da).items, 0, sizeof(&(da).items[0]) * (da).capacity); \
    } while (0)

/**
 * Frees a dynamic array
 * see_da_free<T>(DynamicArray<T> &da)
 */
#define see_da_free(da)       \
    do                        \
    {                         \
        see_free((da).items); \
        (da).items = NULL;    \
        (da).count = 0;       \
        (da).capacity = 0;    \
    } while (0)

#endif