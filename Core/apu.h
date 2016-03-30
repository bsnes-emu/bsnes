#ifndef apu_h
#define apu_h
#include <stdbool.h>
#include <stdint.h>

/* Divides nicely and never overflows with 4 channels */
#define MAX_CH_AMP 0x1E00
#define CH_STEP (0x1E00/0xF)


struct GB_gameboy_s;
typedef struct GB_gameboy_s GB_gameboy_t;

/* Not all used on all channels */
typedef struct
{
    double phase;
    double frequency;
    int16_t amplitude;
    int16_t start_amplitude;
    double duty;
    double sound_length; /* In seconds */
    bool stop_on_length;
    unsigned char envelope_steps;
    unsigned char cur_envelope_steps;
    signed int envelope_direction;
    unsigned char sweep_steps;
    unsigned char cur_sweep_steps;
    signed int sweep_direction;
    unsigned char sweep_shift;
    bool is_playing;
} GB_apu_channel_t;

typedef struct
{
    GB_apu_channel_t wave_channels[4];
    double envelope_step_timer; /* In seconds */
    double sweep_step_timer; /* In seconds */
    signed char wave_form[32];
    unsigned char wave_shift;
    bool wave_enable;
    uint16_t lfsr;
    bool lfsr_7_bit;
    double left_volume;
    double right_volume;
    bool left_on[4];
    bool right_on[4];
    bool global_enable;
} GB_apu_t;

void apu_render(GB_gameboy_t *gb, unsigned long sample_rate, unsigned long n_samples, int16_t *samples);
void apu_copy_buffer(GB_gameboy_t *gb, int16_t *dest, unsigned int count);
void apu_write(GB_gameboy_t *gb, unsigned char reg, unsigned char value);
unsigned char apu_read(GB_gameboy_t *gb, unsigned char reg);
void apu_init(GB_gameboy_t *gb);
void apu_copy_buffer(GB_gameboy_t *gb, int16_t *dest, unsigned int count);
void apu_run(GB_gameboy_t *gb);

#endif /* apu_h */
