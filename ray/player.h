#pragma once

#include <stddef.h>
#include "raylib.h"
#include "map.h"

#define PLAYER_FOV 60

typedef struct
{
    Vector2 position;
    float direction;
    float rays[PLAYER_FOV];
} Player;

Player player_init(const char *map_data, size_t map_width, size_t map_height);
void player_debug(Player player, const char *preamble);
void player_draw(Player player);
void player_draw_pov(Player player, Rectangle screen);
Player player_update(Player player, float dt, Map map);

#define PLAYER_DEBUG(player) player_debug(player, #player)