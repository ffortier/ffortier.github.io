#ifndef DSP_DA_H
#define DSP_DA_H
#include <string.h>
#include <assert.h>

/**
 * typedef struct {
 *     T* items;
 *     size_t count;
 *     size_t len;
 * } DynamicArray<T>;
 */

/**
 * Extends a dynamic array `da` with an array `data` of size `len`
 * da_extend<T>(DynamicArray<T> &da, T* data, size_t len)
 */
#define da_extend(da, data, len)                                                      \
    do                                                                                \
    {                                                                                 \
        while ((da).count + (len) > (da).capacity)                                    \
        {                                                                             \
            (da).capacity = (da).capacity == 0 ? 100 : (da).capacity * 2;             \
            (da).items = realloc((da).items, sizeof(&(da).items[0]) * (da).capacity); \
            assert((da).items && "Could not realloc memory");                         \
        }                                                                             \
        memcpy(&(da).items[(da).count], data, sizeof(&(da).items[0]) * (len));        \
        (da).count += len;                                                            \
    } while (0)

/**
 * Appends a single item `item` to a dynamic array `da`
 * da_append<T>(DynamicArray<T> &da, T data)
 */
#define da_append(da, item) da_extend(da, &item, 1)

/**
 * Ensures the dynamic has enough capacity and zero initialize the buffer
 * da_init<T>(DynamicArray<T> &da, T data)
 */
#define da_ensure_capacity(da, min_capacity)                                          \
    do                                                                                \
    {                                                                                 \
        if ((da).capacity < min_capacity)                                             \
        {                                                                             \
            (da).capacity = min_capacity;                                             \
            (da).items = realloc((da).items, sizeof(&(da).items[0]) * (da).capacity); \
            assert((da).items && "Could not realloc memory");                         \
        }                                                                             \
    } while (0)

/**
 * Ensures the dynamic has enough capacity and zero initialize the buffer
 * da_init<T>(DynamicArray<T> &da, T data)
 */
#define da_init(da, initial_capacity)                                  \
    do                                                                 \
    {                                                                  \
        da_ensure_capacity(da, initial_capacity);                      \
        (da).count = 0;                                                \
        memset((da).items, 0, sizeof(&(da).items[0]) * (da).capacity); \
    } while (0)

/**
 * Frees a dynamic array
 * da_free<T>(DynamicArray<T> &da)
 */
#define da_free(da)        \
    do                     \
    {                      \
        free((da).items);  \
        (da).items = NULL; \
        (da).count = 0;    \
        (da).capacity = 0; \
    } while (0)

#endif