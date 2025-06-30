#include "../include/chip8.h"

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_MAX_SIZE 4096 - 512

chip8_t chip8_init(renderer_t r)
{
    keyboard_t k = init_keyboard();
    sound_t* s = sound_init(200.0f);
    chip8_t c = {
        .renderer = r,
        .keyboard = k,
        .sound = s,
        .fps = 60,
        .program_counter = 0x200,
        .delay_timer = 0,
        .sound_timer = 0,
        .I = 0,
        .V = {0},
        .memory = {0},
    };

    uint8_t sprites[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    memcpy(c.memory, sprites, sizeof(sprites));

    return c;
}

bool load_rom(chip8_t* c, const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    uint64_t file_size = (uint64_t)ftell(fp);
    rewind(fp);
    if (file_size > ROM_MAX_SIZE) {
        fclose(fp);
        return false;
    }

    uint8_t rom[ROM_MAX_SIZE];
    uint64_t bytes_read = fread(rom, 1, file_size, fp);
    fclose(fp);
    if (bytes_read != file_size) {
        return false;
    }

    memcpy(c->memory + 0x200, rom, file_size);
    return true;
}

void chip8_run(chip8_t* c)
{
    SetTargetFPS(c->fps);
    char pressed_keys[17];
    while (!WindowShouldClose())
    {
        handle_inputs(&c->keyboard);
        keys_to_char(c->keyboard, pressed_keys, 17);
        render(&c->renderer, pressed_keys);
        pressed_keys[0] = '\0';
    }
}
