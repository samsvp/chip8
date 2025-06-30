#include "include/renderer.h"
#include "include/chip8.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage:\n");
        printf("./chip-8.out rom-name:\n");
        return 0;
    }

    renderer_t r = renderer_init(10);
    chip8_t c = chip8_init(r);
    char* rom_name = argv[1];
    if (!load_rom(&c, rom_name)) {
        printf("Could not load the provided rom %s.\n", rom_name);
        return 1;
    }
    chip8_run(&c, 10);
    renderer_deinit(&r);

    return 0;
}
