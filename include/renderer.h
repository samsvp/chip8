#ifndef C8_RENDERER
#define C8_RENDERER

#include <stdbool.h>
#define COLS 64
#define ROWS 64

typedef struct {
    int scale;
    bool display[COLS * ROWS];
} renderer_t;

/**
 * Initializes the renderer and setup raylib
 */
renderer_t renderer_init(int scale);
/**
 * Toggles the pixel in x, y on or off and returns
 * true if a pixel was erased and false if nothing was erased
 */
bool set_pixel(renderer_t* r, int x, int y);
/**
 * Clears the display
 */
void clear(renderer_t* r);
/**
 * Renders the pixels in r.display
 */
void render(renderer_t* r, char* keys);
/**
 * Deinitializes the renderer
 */
void renderer_deinit(renderer_t* r);

#endif
