#include "../include/sound.h"

#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_SAMPLES_PER_UPDATE 4096

sound_t* s;
void audio_input_callback(void* buffer, uint32_t frames)
{
    static float sine_idx = 0.0f;

    s->audio_frequency = s->frequency + 0.95f * (s->audio_frequency - s->frequency);
    float incr = s->audio_frequency / 44100.0f;
    uint16_t* b = buffer;

    for (uint32_t i = 0; i < frames; i++) {
        b[i] = (uint16_t)(32000.0f * sinf(2 * PI * sine_idx));
        sine_idx += incr;
        if (sine_idx > 1.0f) {
            sine_idx = -1.0f;
        }
    }
}

sound_t* sound_init(float frequency)
{
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(MAX_SAMPLES_PER_UPDATE);

    s = malloc(sizeof(sound_t));
    s->stream = LoadAudioStream(44100, 16, 1);
    s->audio_frequency = frequency;
    s->frequency = frequency;

    SetAudioStreamCallback(s->stream, audio_input_callback);

    return s;
}


void play(sound_t* s)
{
    PlayAudioStream(s->stream);
}

void stop(sound_t* s)
{
    StopAudioStream(s->stream);
}

void sound_deinit(sound_t* s)
{
    free(s);

    UnloadAudioStream(s->stream);
    CloseAudioDevice();
}
