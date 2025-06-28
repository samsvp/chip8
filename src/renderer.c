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
    // wrap the pixel value
    while (x > COLS) {
        x -= COLS;
    }
    while (x < 0) {
        x += COLS;
    }
    while (y > ROWS) {
        y -= ROWS;
    }
    while (y < 0) {
        y += ROWS;
    }

    int loc = x + y * COLS;
    // toggle
    r->display[loc] ^= 1;
    return !r->display[loc];
}

void clear(renderer_t* r)
{
    memset(r->display, false, sizeof r->display);
}

void render(renderer_t* r)
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
    EndDrawing();
}

void renderer_deinit(renderer_t* r)
{
    (void)r;
    CloseWindow();
}
