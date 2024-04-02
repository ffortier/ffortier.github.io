#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "raylib.h"
#include "raymath.h"
#include "map.h"
#include "player.h"
#include "common.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Map map;
Player player;
Camera2D camera;
Rectangle screen;

void setup(void)
{
    // map = map_init(&MAP_DATA[0], MAP_WIDTH, MAP_HEIGHT);
    map = map_maze(4, 10, 10);
    player = player_init(map.data, map.width, map.height);

    screen = (Rectangle){
        .x = 0,
        .y = 0,
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
    };

    camera.zoom = 16.0;
    camera.offset = (Vector2){.x = SCREEN_WIDTH / 2.0, .y = SCREEN_HEIGHT / 2.0};
}

void draw(void)
{
    float dt = GetFrameTime();
    player = player_update(player, dt, map);

    BeginDrawing();
    ClearBackground(BLACK);

    // player pov
    DrawRectangleGradientV(screen.x, screen.height / 2.0, screen.width, screen.height / 2.0, BLACK, GREEN);
    player_draw_pov(player, screen);

    // minimap
    camera.rotation = player.direction;
    camera.target = player.position;
    BeginMode2D(camera);
    map_draw(map);
    player_draw(player);
    EndMode2D();

    EndDrawing();
}

int main(void)
{
    InitWindow(800, 600, "Hello");
    SetTargetFPS(60);

    setup();

    while (!WindowShouldClose())
    {
        draw();
    }

    CloseWindow();

    return 0;
}
