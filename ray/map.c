#include "map.h"
#include "raylib.h"
#include "common.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

Map map_init(const char *data, size_t width, size_t height)
{
    return (Map){
        .data = data,
        .width = width,
        .height = height,
    };
}

Map map_maze(size_t block_size, size_t grid_width, size_t grid_height)
{
    size_t width = block_size * grid_width + 2;
    size_t height = block_size * grid_height + 2;

    size_t data_length = width * height;
    char *data = malloc(width * height);
    memset(data, ' ', data_length);

    for (size_t x = 0; x < width; x++)
    {
        data[x] = data[x + (height - 1) * width] = '#';
    }

    for (size_t y = 0; y < height; y++)
    {
        data[y * width + 0] = data[y * width + width - 1] = '#';
    }

    data[2 * width + 2] = '$';

    return map_init(data, width, height);
}

void map_debug(Map map, const char *name)
{
    printf("Map %s {\n", name);
    printf("  .data = <...>,\n");
    printf("  .width = %zu,\n", map.width);
    printf("  .height = %zu,\n", map.height);
    printf("}\n");
}

#define CELL_RECT(x, y) ((Rectangle){.x = (float)x - .5, .y = (float)y - .5, .width = 1.0, .height = 1.0})

void map_draw(Map map)
{
    Vector2 size = (Vector2){.x = 1.0, .y = 1.0};

    for (size_t y = 0; y < map.height; y++)
    {
        for (size_t x = 0; x < map.width; x++)
        {
            size_t i = y * map.width + x;
            switch (map.data[i])
            {
            case '#':
            {
                Rectangle rect = CELL_RECT(x, y);
                Vector2 pos = (Vector2){.x = rect.x, .y = rect.y};
                DrawRectangleV(pos, size, RED);
                break;
            }
            }
        }
    }
}

HitTest map_test(Map map, Rectangle hitbox)
{
    int x1 = (int)hitbox.x;
    int y1 = (int)hitbox.y;
    int x2 = (int)ceilf(hitbox.x + hitbox.width);
    int y2 = (int)ceilf(hitbox.y + hitbox.height);

    HitTest test = HIT_EMPTY;

    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            int i = y * map.width + x;
            Rectangle rect = CELL_RECT(x, y);

            switch (map.data[i])
            {
            case '#':
                if (CheckCollisionRecs(hitbox, rect))
                {
                    test |= HIT_WALL;
                }
                break;
            }
        }
    }

    return test;
}