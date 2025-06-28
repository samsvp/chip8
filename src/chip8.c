#include "../include/chip8.h"

#include <raylib.h>


chip8_t chip8_init(renderer_t r)
{
    keyboard_t k = init_keyboard();
    chip8_t c = {
        .renderer = r,
        .keyboard = k,
        .fps = 60,
    };

    return c;
}

void chip8_run(chip8_t* c)
{
    SetTargetFPS(c->fps);
    while (!WindowShouldClose())
    {
        handle_inputs(&c->keyboard);
        render(&c->renderer);
    }
}
