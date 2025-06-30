#ifndef C8_KEYBOARD
#define C8_KEYBOARD

#include <stdbool.h>
#define NUM_KEYS 16

typedef struct {
    int keys[NUM_KEYS];
    int length;
} keypresses_t;

typedef struct {
    keypresses_t keypresses;
    void (*on_next_key_press)(void* ctx, int);
    void* ctx;
} keyboard_t;

keyboard_t init_keyboard(void);
void handle_inputs(keyboard_t* k);
bool is_key_pressed(keyboard_t k, int key_code);
void keys_to_char(keyboard_t k, char* buffer, int length);

#endif
