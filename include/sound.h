#ifndef C8_SOUND
#define C8_SOUND

#include <raylib.h>
#include <stdint.h>

#define AUDIO_BUFFER_SIZE 512

typedef struct {
    float frequency;
    float audio_frequency;
    int16_t buffer[AUDIO_BUFFER_SIZE];
    AudioStream stream;
} sound_t;

sound_t* sound_init(float frequency);
void play(sound_t* s);
void stop(sound_t* s);
void sound_deinit(sound_t* s);

#endif
