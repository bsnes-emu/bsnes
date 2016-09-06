#ifndef apu_h
#define apu_h
#include <stdbool.h>
#include <stdint.h>

/* Divides nicely and never overflows with 4 channels */
#define MAX_CH_AMP 0x1E00
#define CH_STEP (0x1E00/0xF)

#include "save_struct.h"

struct GB_gameboy_s;
typedef struct GB_gameboy_s GB_gameboy_t;

typedef struct
{
    int16_t left;
    int16_t right;
} GB_sample_t;

/* Not all used on all channels */
typedef struct
{
    GB_aligned_double phase;
    GB_aligned_double frequency;
    GB_aligned_double duty;
    GB_aligned_double sound_length; /* In seconds */
    int16_t amplitude;
    int16_t start_amplitude;
    bool stop_on_length;
    uint8_t envelope_steps;
    uint8_t cur_envelope_steps;
    signed int envelope_direction;
    uint8_t sweep_steps;
    uint8_t cur_sweep_steps;
    signed int sweep_direction;
    uint8_t sweep_shift;
    bool is_playing;
} GB_apu_channel_t;

typedef struct
{
    GB_apu_channel_t wave_channels[4];
    GB_aligned_double envelope_step_timer; /* In seconds */
    GB_aligned_double sweep_step_timer; /* In seconds */
    int8_t wave_form[32];
    uint8_t wave_shift;
    bool wave_enable;
    uint16_t lfsr;
    bool lfsr_7_bit;
    double left_volume;
    double right_volume;
    bool left_on[4];
    bool right_on[4];
    bool global_enable;
    uint16_t NRX3_X4_temp[3];
} GB_apu_t;

void GB_apu_render(GB_gameboy_t *gb, unsigned int sample_rate, unsigned int n_samples, GB_sample_t *samples);
void GB_apu_copy_buffer(GB_gameboy_t *gb, GB_sample_t *dest, unsigned int count);
void GB_apu_write(GB_gameboy_t *gb, uint8_t reg, uint8_t value);
uint8_t GB_apu_read(GB_gameboy_t *gb, uint8_t reg);
void GB_apu_init(GB_gameboy_t *gb);
void GB_apu_run(GB_gameboy_t *gb);

#endif /* apu_h */
