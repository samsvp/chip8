#include "include/renderer.h"
#include "include/chip8.h"
#include <stdio.h>

int main(void)
{
    renderer_t r = renderer_init(10);

    chip8_t c = chip8_init(r);
    if (!load_rom(&c, "roms/BLINKY")) {
        printf("Could not load the provided rom.");
        return 1;
    }
    chip8_run(&c, 10);
    renderer_deinit(&r);

    return 0;
}
