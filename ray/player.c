#include "player.h"
#include "raymath.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SPEED 10.0
#define PLAYER_ROTATION 45.0
#define MAX_RAY_LENGTH 25.0

static Vector2 forward = {
    .x = 0.0,
    .y = -1.0,
};

static float player_cast_ray(Vector2 start_position, float direction, Map map)
{
    for (float d = 0; d < MAX_RAY_LENGTH; d += 1.0)
    {
        Vector2 position = Vector2Add(start_position, Vector2Rotate(Vector2Scale(forward, d), direction * -DEG2RAD));
        Rectangle hitbox = {
            .x = position.x,
            .y = position.y,
            .width = 0,
            .height = 0,
        };

        if (map_test(map, hitbox))
        {
            for (float pull_out = 0.0; pull_out < 1.0; pull_out += 0.05)
            {

                position = Vector2Add(start_position, Vector2Rotate(Vector2Scale(forward, d - pull_out), direction * -DEG2RAD));
                hitbox = (Rectangle){
                    .x = position.x,
                    .y = position.y,
                    .width = 0,
                    .height = 0,
                };
                if (!map_test(map, hitbox))
                {
                    break;
                }
            }
            return Vector2Distance(start_position, position);
        }
    }

    return MAX_RAY_LENGTH;
}

static void player_update_rays(Player *player, Map map)
{
    for (size_t i = 0; i < PLAYER_FOV; i++)
    {
        float angle = (float)i - PLAYER_FOV / 2.0;

        player->rays[i] = player_cast_ray(player->position, player->direction + angle, map);
    }
}

Player player_init(const char *map_data, size_t map_width, size_t map_height)
{
    Vector2 position = {0};

    for (size_t y = 0; y < map_height; y++)
    {
        for (size_t x = 0; x < map_width; x++)
        {
            size_t i = y * map_width + x;

            if (map_data[i] == '$')
            {
                position.x = (float)x;
                position.y = (float)y;
                break;
            }
        }
    }

    return (Player){
        .position = position,
        .direction = 0.0,
        .rays = {0},
    };
}

Player player_update(Player player, float dt, Map map)
{
    if (IsKeyDown(KEY_RIGHT))
    {
        player.direction -= PLAYER_ROTATION * dt;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player.direction += PLAYER_ROTATION * dt;
    }

    Vector2 movement = {0};

    if (IsKeyDown(KEY_A))
    {
        movement.x = -1;
    }
    else if (IsKeyDown(KEY_D))
    {
        movement.x = 1;
    }

    if (IsKeyDown(KEY_W))
    {
        movement.y = -1;
    }
    else if (IsKeyDown(KEY_S))
    {
        movement.y = 1;
    }

    Vector2 position = Vector2Add(player.position, Vector2Rotate(Vector2Scale(movement, PLAYER_SPEED * dt), player.direction * -DEG2RAD));

    Rectangle hitbox = {
        .x = position.x - .5,
        .y = position.y - .5,
        .width = 1.0,
        .height = 1.0,
    };

    if (!map_test(map, hitbox))
    {
        player.position = position;
    }

    player_update_rays(&player, map);

    return player;
}

void player_debug(Player player, const char *preamble)
{
    printf("%s {\n", preamble);
    vector2_debug(player.position, "  .position =");
    printf("  .direction = %.2f,\n", player.direction);
    printf("}\n");
}

void player_draw(Player player)
{
    DrawCircleV(player.position, 0.5, GREEN);

    for (size_t i = 0; i < PLAYER_FOV; i++)
    {
        float angle = player.direction + (float)i - PLAYER_FOV / 2.0;
        Vector2 ray_vec = Vector2Add(player.position, Vector2Rotate(Vector2Scale(forward, player.rays[i]), angle * -DEG2RAD));

        DrawLineV(player.position, ray_vec, WHITE);
    }
}

void player_draw_pov(Player player, Rectangle screen)
{
    float slice_width = screen.width / (float)PLAYER_FOV;

    for (size_t i = 0; i < PLAYER_FOV; i++)
    {

        float distance = player.rays[i] * cosf(((float)i - PLAYER_FOV / 2.0) * DEG2RAD);
        float value = Lerp(1.0, 0.0, distance / MAX_RAY_LENGTH);
        float height = Lerp(screen.height, screen.height / 4, distance / MAX_RAY_LENGTH);

        Color color = ColorFromHSV(0, 0, value);

        DrawRectangle(screen.x + (PLAYER_FOV - i - 1) * slice_width, (screen.height - height) / 2, ceilf(slice_width), height, color);
    }
}