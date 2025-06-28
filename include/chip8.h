#ifndef C8_CHIP8
#define C8_CHIP8

#include "keyboard.h"
#include "renderer.h"

typedef struct {
    renderer_t renderer;
    keyboard_t keyboard;
    int fps;
    double previous_time;
    double current_time;
} chip8_t;

/**
 * Initializes a new chip 8 struct with the given renderer.
 */
chip8_t chip8_init(renderer_t r);

/**
 * Start running the chip 8 VM
 */
void chip8_run(chip8_t* c);

#endif
