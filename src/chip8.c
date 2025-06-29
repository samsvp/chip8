#include "../include/chip8.h"

#include <raylib.h>


chip8_t chip8_init(renderer_t r)
{
    keyboard_t k = init_keyboard();
    sound_t* s = sound_init(200.0f);
    chip8_t c = {
        .renderer = r,
        .keyboard = k,
        .sound = s,
        .fps = 60,
    };

    return c;
}

void chip8_run(chip8_t* c)
{
    SetTargetFPS(c->fps);
    char pressed_keys[17];
    while (!WindowShouldClose())
    {
        handle_inputs(&c->keyboard);

        if (IsKeyPressed(KEY_SPACE)) {
            play(c->sound);
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            stop(c->sound);
        }

        keys_to_char(c->keyboard, pressed_keys, 17);
        render(&c->renderer, pressed_keys);
        pressed_keys[0] = '\0';
    }
}
