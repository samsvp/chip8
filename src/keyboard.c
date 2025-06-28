#include "../include/keyboard.h"
#include <raylib.h>
#include <string.h>


void kp_add(keypresses_t* kp, int k)
{
    if (kp->length >= NUM_KEYS) {
        return;
    }

    kp->keys[kp->length++] = k;
}

void kp_remove(keypresses_t* kp, int k)
{
    for (int i = 0; i < kp->length; i++) {
        if (kp->keys[i] != k) {
            continue;
        }
        kp->keys[i] = kp->keys[kp->length];
        kp->length--;
        return;
    }
}

void kp_clear(keypresses_t* kp)
{
    kp->length = 0;
}

keyboard_t init_keyboard(void)
{
    keypresses_t kp = {
        .keys = {0},
        .length = 0
    };

    keyboard_t k = {
        .keypresses = kp,
        .on_next_key_press = NULL
    };

    return k;
}

void handle_inputs(keyboard_t* k)
{
    if (IsKeyPressed(KEY_ONE)) kp_add(&k->keypresses, 0x1);
    if (IsKeyPressed(KEY_TWO)) kp_add(&k->keypresses, 0x2);
    if (IsKeyPressed(KEY_THREE)) kp_add(&k->keypresses, 0x3);
    if (IsKeyPressed(KEY_FOUR)) kp_add(&k->keypresses, 0xc);
    if (IsKeyPressed(KEY_Q)) kp_add(&k->keypresses, 0x4);
    if (IsKeyPressed(KEY_W)) kp_add(&k->keypresses, 0x5);
    if (IsKeyPressed(KEY_E)) kp_add(&k->keypresses, 0x6);
    if (IsKeyPressed(KEY_R)) kp_add(&k->keypresses, 0xd);
    if (IsKeyPressed(KEY_A)) kp_add(&k->keypresses, 0x7);
    if (IsKeyPressed(KEY_S)) kp_add(&k->keypresses, 0x8);
    if (IsKeyPressed(KEY_D)) kp_add(&k->keypresses, 0x9);
    if (IsKeyPressed(KEY_F)) kp_add(&k->keypresses, 0xe);
    if (IsKeyPressed(KEY_Z)) kp_add(&k->keypresses, 0xa);
    if (IsKeyPressed(KEY_X)) kp_add(&k->keypresses, 0x0);
    if (IsKeyPressed(KEY_C)) kp_add(&k->keypresses, 0xb);
    if (IsKeyPressed(KEY_V)) kp_add(&k->keypresses, 0xf);

    if (IsKeyUp(KEY_ONE)) kp_remove(&k->keypresses, 0x1);
    if (IsKeyUp(KEY_TWO)) kp_remove(&k->keypresses, 0x2);
    if (IsKeyUp(KEY_THREE)) kp_remove(&k->keypresses, 0x3);
    if (IsKeyUp(KEY_FOUR)) kp_remove(&k->keypresses, 0xc);
    if (IsKeyUp(KEY_Q)) kp_remove(&k->keypresses, 0x4);
    if (IsKeyUp(KEY_W)) kp_remove(&k->keypresses, 0x5);
    if (IsKeyUp(KEY_E)) kp_remove(&k->keypresses, 0x6);
    if (IsKeyUp(KEY_R)) kp_remove(&k->keypresses, 0xd);
    if (IsKeyUp(KEY_A)) kp_remove(&k->keypresses, 0x7);
    if (IsKeyUp(KEY_S)) kp_remove(&k->keypresses, 0x8);
    if (IsKeyUp(KEY_D)) kp_remove(&k->keypresses, 0x9);
    if (IsKeyUp(KEY_F)) kp_remove(&k->keypresses, 0xe);
    if (IsKeyUp(KEY_Z)) kp_remove(&k->keypresses, 0xa);
    if (IsKeyUp(KEY_X)) kp_remove(&k->keypresses, 0x0);
    if (IsKeyUp(KEY_C)) kp_remove(&k->keypresses, 0xb);
    if (IsKeyUp(KEY_V)) kp_remove(&k->keypresses, 0xf);

    if (k->on_next_key_press != NULL && k->keypresses.length > 0) {
        k->on_next_key_press(k->keypresses.keys[0]);
        k->on_next_key_press = NULL;
    }
}

void keys_to_char(keyboard_t k, char* buffer, int length)
{
    int i;
    for (i = 0; i < k.keypresses.length; i++) {
        // will not fit the buffer
        if (length <= i) {
            return;
        }

        char c = '?';
        switch (k.keypresses.keys[i]) {
            case 0x1: c = '1'; break;
            case 0x2: c = '2'; break;
            case 0x3: c = '3'; break;
            case 0xc: c = 'C'; break;
            case 0x4: c = '4'; break;
            case 0x5: c = '5'; break;
            case 0x6: c = '6'; break;
            case 0xd: c = 'D'; break;
            case 0x7: c = '7'; break;
            case 0x8: c = '8'; break;
            case 0x9: c = '9'; break;
            case 0xe: c = 'E'; break;
            case 0xa: c = 'A'; break;
            case 0x0: c = '0'; break;
            case 0xb: c = 'B'; break;
            case 0xf: c = 'F'; break;
        }
        buffer[i] = c;
    }

    if (length > i) {
        buffer[i] = '\0';
    }
}

bool is_key_pressed(keyboard_t k, int key_code)
{
    for (int i = 0; i < k.keypresses.length; i++) {
        if (k.keypresses.keys[i] == key_code) {
            return true;
        }
    }
    return false;
}
