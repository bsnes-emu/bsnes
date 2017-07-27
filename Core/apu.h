#ifndef apu_h
#define apu_h
#include <stdbool.h>
#include <stdint.h>
#include "gb_struct_def.h"


#ifdef GB_INTERNAL
/* Divides nicely and never overflows with 4 channels and 8 volume levels */
#define MAX_CH_AMP 0x1FFE
#define CH_STEP (MAX_CH_AMP/0xF/7)
#endif

/* Lengths are in either DIV ticks (512Hz, triggered by the DIV register) or
   APU ticks (2MHz, triggered by an internal APU clock) */

typedef struct
{
    int16_t left;
    int16_t right;
} GB_sample_t;

enum GB_CHANNELS {
    GB_SQUARE_1,
    GB_SQUARE_2,
    GB_WAVE,
    GB_NOISE,
    GB_N_CHANNELS
};

typedef struct
{
    bool global_enable;
    uint8_t apu_cycles;
    
    uint8_t samples[GB_N_CHANNELS];
    bool left_enabled[GB_N_CHANNELS];
    bool right_enabled[GB_N_CHANNELS];
    bool is_active[GB_N_CHANNELS];
    
    uint8_t square_carry; // The square channels tick at 1MHz instead of 2,
                          // so we need a carry to divide the signal
    
    uint8_t square_sweep_div; // The DIV-APU ticks are divided by 4 to handle tone sweeping
    uint8_t square_sweep_countdown; // In 128Hz
    uint8_t square_sweep_stop_countdown; // In 2 MHz
    
    struct {
        uint16_t pulse_length; // Reloaded from NRX1 (xorred), in DIV ticks
        uint8_t current_volume; // Reloaded from NRX2
        uint8_t volume_countdown; // Reloaded from NRX2
        uint8_t current_sample_index;
        bool sample_emitted;
        
        uint16_t sample_countdown; // in APU ticks
        uint16_t sample_length; // Reloaded from NRX3, NRX4, in APU ticks
        bool length_enabled; // NRX4

    } square_channels[2];
    
    struct {
        bool enable; // NR30
        uint16_t pulse_length; // Reloaded from NR31 (xorred), in DIV ticks
        uint8_t shift; // NR32
        uint16_t sample_length; // NR33, NR34, in APU ticks
        bool length_enabled; // NR34
        
        uint16_t sample_countdown; // in APU ticks
        uint8_t current_sample_index;
        uint8_t current_sample; // Current sample before shifting.
        
        int8_t wave_form[32];
        bool wave_form_just_read;
    } wave_channel;
} GB_apu_t;

typedef struct {
    unsigned sample_rate;
    
    GB_sample_t *buffer;
    size_t buffer_size;
    size_t buffer_position;
    
    bool stream_started; /* detects first copy request to minimize lag */
    volatile bool copy_in_progress;
    volatile bool lock;
    
    double sample_cycles;
    
    // Samples are NOT normalized to MAX_CH_AMP * 4 at this stage!
    unsigned cycles_since_render;
    unsigned last_update[GB_N_CHANNELS];
    GB_sample_t current_sample[GB_N_CHANNELS];
    GB_sample_t summed_samples[GB_N_CHANNELS];
} GB_apu_output_t;

void GB_set_sample_rate(GB_gameboy_t *gb, unsigned int sample_rate);
void GB_apu_copy_buffer(GB_gameboy_t *gb, GB_sample_t *dest, size_t count);
size_t GB_apu_get_current_buffer_length(GB_gameboy_t *gb);

#ifdef GB_INTERNAL
void GB_apu_write(GB_gameboy_t *gb, uint8_t reg, uint8_t value);
uint8_t GB_apu_read(GB_gameboy_t *gb, uint8_t reg);
void GB_apu_div_event(GB_gameboy_t *gb);
void GB_apu_init(GB_gameboy_t *gb);
void GB_apu_run(GB_gameboy_t *gb);
#endif

#endif /* apu_h */
