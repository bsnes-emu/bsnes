#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <Core/gb.h>

bool GB_audio_is_playing(void);
void GB_audio_set_paused(bool paused);
void GB_audio_clear_queue(void);
unsigned GB_audio_get_frequency(void);
size_t GB_audio_get_queue_length(void);
void GB_audio_queue_sample(GB_sample_t *sample);
bool GB_audio_init(void);
void GB_audio_deinit(void);
const char *GB_audio_driver_name(void);
const char *GB_audio_driver_name_at_index(unsigned index);

typedef struct {
    typeof(GB_audio_is_playing) *audio_is_playing;
    typeof(GB_audio_set_paused) *audio_set_paused;
    typeof(GB_audio_clear_queue) *audio_clear_queue;
    typeof(GB_audio_get_frequency) *audio_get_frequency;
    typeof(GB_audio_get_queue_length) *audio_get_queue_length;
    typeof(GB_audio_queue_sample) *audio_queue_sample;
    typeof(GB_audio_init) *audio_init;
    typeof(GB_audio_deinit) *audio_deinit;
    const char *name;
} GB_audio_driver_t;

#define GB_AUDIO_DRIVER(_name) const GB_audio_driver_t _name##driver = { \
    .audio_is_playing = _audio_is_playing, \
    .audio_set_paused = _audio_set_paused, \
    .audio_clear_queue = _audio_clear_queue, \
    .audio_get_frequency = _audio_get_frequency, \
    .audio_get_queue_length = _audio_get_queue_length, \
    .audio_queue_sample = _audio_queue_sample, \
    .audio_init = _audio_init, \
    .audio_deinit = _audio_deinit, \
    .name = #_name, \
}

#define GB_AUDIO_DRIVER_REF(name) ({extern const GB_audio_driver_t name##driver; &name##driver;})
