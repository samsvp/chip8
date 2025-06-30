#include "../include/chip8.h"

#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_MAX_SIZE (4096 - 512)

chip8_t chip8_init(renderer_t r)
{
    keyboard_t k = init_keyboard();
    sound_t* s = sound_init(200.0f);
    chip8_t c = {
        .renderer = r,
        .keyboard = k,
        .sound = s,
        .fps = 60,
        .pc = 0x200,
        .sp = -1,
        .delay_timer = 0,
        .sound_timer = 0,
        .I = 0,
        .V = {0},
        .memory = {0},
    };

    uint8_t sprites[80] = {
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

typedef struct {
    chip8_t* c;
    uint8_t x;
} keypress_ctx;

static void on_next_keypress(void* ctx, int keycode)
{
    printf("hi, keycode is %d\n", keycode);
    keypress_ctx* kc = ctx;
    kc->c->V[kc->x] = (uint8_t)keycode;
    free(ctx);
}

static void exec_opcode(chip8_t* c, uint16_t opcode)
{
    c->pc += 2;

    uint8_t x = (uint8_t)((opcode & 0x0F00) >> 8);
    uint8_t y = (uint8_t)((opcode & 0x00F0) >> 4);

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    clear(&c->renderer);
                    break;
                case 0x00EE:
                    c->pc = c->stack[c->sp];
                    c->sp--;
                    break;
            }

            break;
        case 0x1000:
            c->pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            c->stack[++c->sp] = c->pc;
            c->pc = opcode & 0x0FFF;
            break;
        case 0x3000:
            if (c->V[x] == (uint8_t)(opcode & 0x00FF)) {
                c->pc += 2;
            }
            break;
        case 0x4000:
            if (c->V[x] != (uint8_t)(opcode & 0x00FF)) {
                c->pc += 2;
            }
            break;
        case 0x5000:
            if (c->V[x] == c->V[y]) {
                c->pc += 2;
            }
            break;
        case 0x6000:
            c->V[x] = (uint8_t)(opcode & 0x0FF);
            break;
        case 0x7000:
            c->V[x] += (uint8_t)(opcode & 0x0FF);
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0:
                    c->V[x] = c->V[y];
                    break;
                case 0x1:
                    c->V[x] |= c->V[y];
                    break;
                case 0x2:
                    c->V[x] &= c->V[y];
                    break;
                case 0x3:
                    c->V[x] ^= c->V[y];
                    break;
                case 0x4: {
                    uint16_t res = (uint16_t)c->V[x] + (uint16_t)c->V[y];
                    c->V[x] = (uint8_t)res;
                    c->V[0xF] = res > 255 ? 1 : 0;
                    break;
                }
                case 0x5: {
                    uint8_t carry = c->V[y] <= c->V[x];
                    c->V[x] -= c->V[y];
                    c->V[0xF] = carry;
                    break;
                }
                case 0x6: {
                    uint8_t carry = c->V[x] & 0x1;
                    c->V[x] >>= 1;
                    c->V[0xF] = carry;
                }
                    break;
                case 0x7: {
                    uint8_t carry = c->V[x] <= c->V[y];
                    c->V[x] = c->V[y] - c->V[x];
                    c->V[0xF] = carry;
                    break;
                }
                case 0xE: {
                    uint8_t carry = (c->V[x] >> 7) & 0x1;
                    c->V[x] <<= 1;
                    c->V[0xF] = carry;
                    break;
                }
            }

            break;
        case 0x9000:
            if (c->V[x] != c->V[y]) {
                c->pc += 2;
            }
            break;
        case 0xA000:
            c->I = opcode & 0x0FFF;
            break;
        case 0xB000:
            c->pc = c->V[0] + (opcode & 0x0FFF);
            break;
        case 0xC000:
            c->V[x] = (rand() % 256) & (opcode & 0xFF);
            break;
        case 0xD000: {
            uint8_t w = 8;
            uint8_t h = opcode & 0xF;
            c->V[0xF] = 0;
            for (uint8_t row = 0; row < h; row++) {
                uint8_t sprite = c->memory[c->I+row];

                for (uint8_t col = 0; col < w; col++) {
                    if ((sprite & 0x80) > 0) {
                        if (set_pixel(&c->renderer, c->V[x] + col, c->V[y] + row)) {
                            c->V[0xF] = 1;
                        }
                    }
                    // Shift the sprite left 1. This will move the next next col/bit
                    // of the sprite into the first position.
                    // Ex. 10010000 << 1 will become 0010000
                    sprite <<=1;
                }
            }
            break;
        }
        case 0xE000:
            switch (opcode & 0xFF) {
                case 0x9E:
                    if (is_key_pressed(c->keyboard, c->V[x])) {
                        c->pc += 2;
                    }
                    break;
                case 0xA1:
                    if (!is_key_pressed(c->keyboard, c->V[x])) {
                        c->pc += 2;
                    }
                    break;
            }

            break;
        case 0xF000:
            switch (opcode & 0xFF) {
                case 0x07:
                    c->V[x] = c->delay_timer;
                    break;
                case 0x0A: {
                    keypress_ctx* k = malloc(sizeof(keypress_ctx));
                    k->c = c;
                    k->x = x;
                    c->paused = true;
                    c->keyboard.ctx = k;
                    c->keyboard.on_next_key_press = on_next_keypress;
                    break;
                }
                case 0x15:
                    c->delay_timer = c->V[x];
                    break;
                case 0x18:
                    c->sound_timer = c->V[x];
                    break;
                case 0x1E:
                    c->I += (uint16_t)c->V[x];
                    break;
                case 0x29:
                    c->I = ((uint16_t)c->V[x]) * 5;
                    break;
                case 0x33:
                    c->memory[c->I] = c->V[x] / 100;
                    c->memory[c->I + 1] = c->V[x] % 100 / 10;
                    c->memory[c->I + 2] = c->V[x] % 10;
                    break;
                case 0x55:
                    for (uint8_t i = 0; i <= x; i++) {
                        c->memory[c->I + i] = c->V[i];
                    }
                    break;
                case 0x65:
                    for (uint8_t i = 0; i <= x; i++) {
                        c->V[i] = c->memory[c->I + i];
                    }
                    break;
            }

            break;

        default:
            break;
    }
}

static void cycle(chip8_t* c, int instructions_per_cycle)
{
    if (!c->paused) for (int i = 0; i < instructions_per_cycle; i++) {
        uint16_t opcode = ((uint16_t)(c->memory[c->pc]) << 8) | (c->memory[c->pc + 1]);
        exec_opcode(c, opcode);
    }

    // update timers
    if (!c->paused) {
        if (c->delay_timer > 0) {
            c->delay_timer--;
        }
        if (c->sound_timer > 0) {
            c->sound_timer--;
        }
    }

    if (c->sound_timer > 0) {
        play(c->sound);
    } else {
        stop(c->sound);
    }
}

void chip8_run(chip8_t* c, int speed)
{
    SetTargetFPS(c->fps);
    char pressed_keys[17];
    while (!WindowShouldClose())
    {
        cycle(c, speed);
        handle_inputs(&c->keyboard);
        keys_to_char(c->keyboard, pressed_keys, 17);
        render(&c->renderer, pressed_keys);
        pressed_keys[0] = '\0';
    }
}
