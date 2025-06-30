#include "../include/renderer.h"
#include <string.h>
#include <raylib.h>

renderer_t renderer_init(int scale)
{
    InitWindow(scale * COLS, scale * ROWS, "CHIP 8");

    renderer_t r = {
        .scale = scale,
        .display = {0},
    };

    return r;
}

bool set_pixel(renderer_t* r, int x, int y)
{
    x = ((x % COLS) + COLS) % COLS;
    y = ((y % ROWS) + ROWS) % ROWS;

    int loc = x + y * COLS;
    // toggle
    r->display[loc] ^= 1;
    return !r->display[loc];
}

void clear(renderer_t* r)
{
    memset(r->display, false, sizeof r->display);
}

void render(renderer_t* r, char* keys)
{
    int rect_size = r->scale;

    ClearBackground(RAYWHITE);
    BeginDrawing();
    for (int i = 0; i < COLS * ROWS; i++) {
        int x = (i % COLS) * rect_size;
        int y = i / COLS * rect_size;

        if (r->display[i]) {
            DrawRectangle(x, y, rect_size, rect_size, BLACK);
        }
    }
    DrawText(keys, 0, 0, 30, RED);
    EndDrawing();
}

void renderer_deinit(renderer_t* r)
{
    (void)r;
    CloseWindow();
}
