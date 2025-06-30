#ifndef C8_CHIP8
#define C8_CHIP8

#include "keyboard.h"
#include "renderer.h"
#include "sound.h"
#include <stdint.h>

typedef struct {
    renderer_t renderer;
    keyboard_t keyboard;
    sound_t* sound;
    int fps;

    uint8_t memory[4096];

    // registers
    uint8_t V[16];
    uint16_t I;

    uint16_t pc;
    uint8_t sp;

    uint16_t stack[16];

    uint32_t delay_timer;
    uint32_t sound_timer;

    bool paused;
} chip8_t;

/**
 * Initializes a new chip 8 struct with the given renderer.
 */
chip8_t chip8_init(renderer_t r);

/**
 * Start running the chip 8 VM
 */
void chip8_run(chip8_t* c, int speed);
bool load_rom(chip8_t* c, const char* path);

#endif
