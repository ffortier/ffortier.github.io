#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "raylib.h"

#define optional_type(T) \
    struct               \
    {                    \
        bool present;    \
        T value;         \
    }

#define some(OptionalType, val) \
    (OptionalType) { .present = true, .value = val }

#define none(OptionalType) \
    (OptionalType) { 0 }

void vector2_debug(Vector2 vec, const char *preamble);
void rect_debug(Rectangle rect, const char *preamble);

#define VEC2_DEBUG(vec) vector2_debug(vec, "Vector2 " #vec)
#define RECT_DEBUG(vec) vector2_debug(vec, "Rectangle " #vec)

#define return_defer(value) \
    do                      \
    {                       \
        result = value;     \
        goto defer;         \
    } while (false)
