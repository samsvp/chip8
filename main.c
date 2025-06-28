#include "include/renderer.h"
#include "include/chip8.h"

int main(void)
{
    renderer_t r = renderer_init(10);

    set_pixel(&r, 0, 0);
    set_pixel(&r, 5, 5);

    chip8_t c = chip8_init(r);
    chip8_run(&c);
    renderer_deinit(&r);

    return 0;
}
