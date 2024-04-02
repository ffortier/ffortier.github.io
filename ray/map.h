#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "raylib.h"

extern const char MAP_DATA[];
extern const size_t MAP_WIDTH;
extern const size_t MAP_HEIGHT;

typedef struct
{
    const char *data;
    size_t width;
    size_t height;
} Map;

typedef enum
{
    HIT_EMPTY = 0,
    HIT_WALL = 0b0001,
} HitTest;

Map map_maze(size_t block_size, size_t grid_width, size_t grid_height);
Map map_init(const char *data, size_t width, size_t height);
void map_debug(Map map, const char *name);
void map_draw(Map map);
HitTest map_test(Map map, Rectangle hitbox);

#define MAP_DEBUG(map) map_debug(map, #map)
