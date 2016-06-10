#include <stdint.h>
#include <math.h>
#include <string.h>
#include "apu.h"
#include "gb.h"

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a > _b ? _a : _b; })

#define min(a,b) \
({ __typeof__ (a) _a = (a); \
__typeof__ (b) _b = (b); \
_a < _b ? _a : _b; })

static __attribute__((unused)) int16_t generate_sin(double phase, int16_t amplitude)
{
    return (int16_t)(sin(phase) * amplitude);
}

static int16_t generate_square(double phase, int16_t amplitude, double duty)
{
    if (fmod(phase, 2 * M_PI) > duty * 2 * M_PI) {
        return amplitude;
    }
    return 0;
}

static int16_t generate_wave(double phase, int16_t amplitude, signed char *wave, unsigned char shift)
{
    phase = fmod(phase, 2 * M_PI);
    return ((wave[(int)(phase / (2 * M_PI) * 32)]) >> shift) * (int)amplitude / 0xF;
}

static int16_t generate_noise(double phase, int16_t amplitude, uint16_t lfsr)
{
    if (lfsr & 1) {
        return amplitude;
    }
    return 0;
}

static int16_t step_lfsr(uint16_t lfsr, bool uses_7_bit)
{
    bool xor = (lfsr & 1) ^ ((lfsr & 2) >> 1);
    lfsr >>= 1;
    if (xor) {
        lfsr |= 0x4000;
    }
    if (uses_7_bit) {
        lfsr &= ~0x40;
        if (xor) {
            lfsr |= 0x40;
        }
    }
    return lfsr;
}

/* General Todo: The APU emulation seems to fail many accuracy tests. It might require a rewrite with
   these tests in mind. */

void apu_render(GB_gameboy_t *gb, unsigned long sample_rate, unsigned long n_samples, GB_sample_t *samples)
{
    for (; n_samples--; samples++) {
        samples->left = samples->right = 0;
        if (!gb->apu.global_enable) {
            continue;
        }

        gb->io_registers[GB_IO_PCM_12] = 0;
        gb->io_registers[GB_IO_PCM_34] = 0;

        {
            int16_t sample = generate_square(gb->apu.wave_channels[0].phase,
                                             gb->apu.wave_channels[0].amplitude,
                                             gb->apu.wave_channels[0].duty);
            if (gb->apu.left_on [0]) samples->left  += sample;
            if (gb->apu.right_on[0]) samples->right += sample;
            gb->io_registers[GB_IO_PCM_12] = ((int)sample) * 0xF / MAX_CH_AMP;
        }

        {
            int16_t sample = generate_square(gb->apu.wave_channels[1].phase,
                                             gb->apu.wave_channels[1].amplitude,
                                             gb->apu.wave_channels[1].duty);
            if (gb->apu.left_on [1]) samples->left  += sample;
            if (gb->apu.right_on[1]) samples->right += sample;
            gb->io_registers[GB_IO_PCM_12] |= (((int)sample) * 0xF / MAX_CH_AMP) << 4;
        }

        {
            int16_t sample = generate_wave(gb->apu.wave_channels[2].phase,
                                           MAX_CH_AMP,
                                           gb->apu.wave_form,
                                           gb->apu.wave_shift);
            if (gb->apu.left_on [2]) samples->left  += sample;
            if (gb->apu.right_on[2]) samples->right += sample;
            gb->io_registers[GB_IO_PCM_34] = ((int)sample) * 0xF / MAX_CH_AMP;
        }

        {
            int16_t sample = generate_noise(gb->apu.wave_channels[3].phase,
                                            gb->apu.wave_channels[3].amplitude,
                                            gb->apu.lfsr);
            if (gb->apu.left_on [3]) samples->left  += sample;
            if (gb->apu.right_on[3]) samples->right += sample;
            gb->io_registers[GB_IO_PCM_34] |= (((int)sample) * 0xF / MAX_CH_AMP) << 4;
        }

        samples->left *= gb->apu.left_volume;
        samples->right *= gb->apu.right_volume;

        for (unsigned char i = 0; i < 4; i++) {
            /* Phase */
            gb->apu.wave_channels[i].phase += 2 * M_PI * gb->apu.wave_channels[i].frequency / sample_rate;
            while (gb->apu.wave_channels[i].phase >= 2 * M_PI) {
                if (i == 3) {
                    gb->apu.lfsr = step_lfsr(gb->apu.lfsr, gb->apu.lfsr_7_bit);
                }
                gb->apu.wave_channels[i].phase -= 2 * M_PI;
            }
            /* Stop on Length */
            if (gb->apu.wave_channels[i].stop_on_length) {
                if (gb->apu.wave_channels[i].sound_length > 0) {
                    gb->apu.wave_channels[i].sound_length -= 1.0 / sample_rate;
                }
                if (gb->apu.wave_channels[i].sound_length <= 0) {
                    gb->apu.wave_channels[i].amplitude = 0;
                    gb->apu.wave_channels[i].is_playing = false;
                    gb->apu.wave_channels[i].sound_length = i == 2? 1 : 0.25;
                }
            }
        }

        gb->apu.envelope_step_timer += 1.0 / sample_rate;
        if (gb->apu.envelope_step_timer >= 1.0 / 64) {
            gb->apu.envelope_step_timer -= 1.0 / 64;
            for (unsigned char i = 0; i < 4; i++) {
                if (gb->apu.wave_channels[i].envelope_steps && !--gb->apu.wave_channels[i].cur_envelope_steps) {
                    gb->apu.wave_channels[i].amplitude = min(max(gb->apu.wave_channels[i].amplitude + gb->apu.wave_channels[i].envelope_direction * CH_STEP, 0), MAX_CH_AMP);
                    gb->apu.wave_channels[i].cur_envelope_steps = gb->apu.wave_channels[i].envelope_steps;
                }
            }
        }

        gb->apu.sweep_step_timer += 1.0 / sample_rate;
        if (gb->apu.sweep_step_timer >= 1.0 / 128) {
            gb->apu.sweep_step_timer -= 1.0 / 128;
            if (gb->apu.wave_channels[0].sweep_steps && !--gb->apu.wave_channels[0].cur_sweep_steps) {

                // Convert back to GB format
                unsigned short temp = (unsigned short) (2048 - 131072 / gb->apu.wave_channels[0].frequency);

                // Apply sweep
                temp = temp + gb->apu.wave_channels[0].sweep_direction *
                       (temp / (1 << gb->apu.wave_channels[0].sweep_shift));
                if (temp > 2047) {
                    temp = 0;
                }

                // Back to frequency
                gb->apu.wave_channels[0].frequency =  131072.0 / (2048 - temp);

                gb->apu.wave_channels[0].cur_sweep_steps = gb->apu.wave_channels[0].sweep_steps;
            }
        }
    }
}

void apu_run(GB_gameboy_t *gb)
{
    static bool should_log_overflow = true;
    while (gb->audio_copy_in_progress);
    double ticks_per_sample = (double) CPU_FREQUENCY / gb->sample_rate;
    while (gb->apu_cycles > ticks_per_sample) {
        GB_sample_t sample = {0, };
        apu_render(gb, gb->sample_rate, 1, &sample);
        gb->apu_cycles -= ticks_per_sample;
        if (gb->audio_position == gb->buffer_size) {
            /*
             if (should_log_overflow && !gb->turbo) {
                gb_log(gb, "Audio overflow\n");
                should_log_overflow = false;
            }
             */
        }
        else {
            gb->audio_buffer[gb->audio_position++] = sample;
            should_log_overflow = true;
        }
    }
}

void apu_copy_buffer(GB_gameboy_t *gb, GB_sample_t *dest, unsigned int count)
{
    gb->audio_copy_in_progress = true;

    if (!gb->audio_stream_started) {
        // Intentionally fail the first copy to sync the stream with the Gameboy.
        gb->audio_stream_started = true;
        gb->audio_position = 0;
    }

    if (count > gb->audio_position) {
        // gb_log(gb, "Audio underflow: %d\n", count - gb->audio_position);
        memset(dest + gb->audio_position, 0, (count - gb->audio_position) * sizeof(*gb->audio_buffer));
        count = gb->audio_position;
    }
    memcpy(dest, gb->audio_buffer, count * sizeof(*gb->audio_buffer));
    memmove(gb->audio_buffer, gb->audio_buffer + count, (gb->audio_position - count) * sizeof(*gb->audio_buffer));
    gb->audio_position -= count;

    gb->audio_copy_in_progress = false;
}

void apu_init(GB_gameboy_t *gb)
{
    memset(&gb->apu, 0, sizeof(gb->apu));
    gb->apu.wave_channels[0].duty = gb->apu.wave_channels[1].duty = 0.5;
    gb->apu.lfsr = 0x7FFF;
    gb->apu.left_volume = 1.0;
    gb->apu.right_volume = 1.0;
    for (int i = 0; i < 4; i++) {
        gb->apu.left_on[i] = gb->apu.right_on[i] = 1;
    }
}

unsigned char apu_read(GB_gameboy_t *gb, unsigned char reg)
{
    /* Todo: what happens when reading from the wave from while it's playing? */

    if (reg == GB_IO_NR52) {
        unsigned char value = 0;
        for (int i = 0; i < 4; i++) {
            value >>= 1;
            if (gb->apu.wave_channels[i].is_playing) {
                value |= 0x8;
            }
        }
        if (gb->apu.global_enable) {
            value |= 0x80;
        }
        value |= 0x70;
        return value;
    }

    static const char read_mask[GB_IO_WAV_END - GB_IO_NR10 + 1] = {
     /* NRX0  NRX1  NRX2  NRX3  NRX4 */
        0x80, 0x3F, 0x00, 0xFF, 0xBF, // NR1X
        0xFF, 0x3F, 0x00, 0xFF, 0xBF, // NR2X
        0x7F, 0xFF, 0x9F, 0xFF, 0xBF, // NR3X
        0xFF, 0xFF, 0x00, 0x00, 0xBF, // NR4X
        0x00, 0x00, 0x70, 0xFF, 0xFF, // NR5X

        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Unused
        // Wave RAM
        0, /* ... */
    };

    if (reg >= GB_IO_WAV_START && reg <= GB_IO_WAV_END && gb->apu.wave_channels[2].is_playing) {
        return (unsigned char)((gb->display_cycles * 22695477 * reg) >> 8); // Semi-random but deterministic
    }

    return gb->io_registers[reg] | read_mask[reg - GB_IO_NR10];
}

void apu_write(GB_gameboy_t *gb, unsigned char reg, unsigned char value)
{
    static const double duties[] = {0.125, 0.25, 0.5, 0.75};
    static uint16_t NRX3_X4_temp[3] = {0};
    unsigned char channel = 0;

    if (!gb->apu.global_enable && reg != GB_IO_NR52) {
        return;
    }

    gb->io_registers[reg] = value;

    switch (reg) {
        case GB_IO_NR10:
        case GB_IO_NR11:
        case GB_IO_NR12:
        case GB_IO_NR13:
        case GB_IO_NR14:
            channel = 0;
            break;
        case GB_IO_NR21:
        case GB_IO_NR22:
        case GB_IO_NR23:
        case GB_IO_NR24:
            channel = 1;
            break;
        case GB_IO_NR33:
        case GB_IO_NR34:
            channel = 2;
            break;
        case GB_IO_NR41:
        case GB_IO_NR42:
            channel = 3;
        default:
            break;
    }

    switch (reg) {
        case GB_IO_NR10:
            gb->apu.wave_channels[channel].sweep_direction = value & 8? -1 : 1;
            gb->apu.wave_channels[channel].cur_sweep_steps =
            gb->apu.wave_channels[channel].sweep_steps = (value & 0x70) >> 4;
            gb->apu.wave_channels[channel].sweep_shift = value & 7;
            break;
        case GB_IO_NR11:
        case GB_IO_NR21:
        case GB_IO_NR41:
            gb->apu.wave_channels[channel].duty = duties[value >> 6];
            gb->apu.wave_channels[channel].sound_length = (64 - (value & 0x3F)) / 256.0;
            if (gb->apu.wave_channels[channel].sound_length == 0) {
                gb->apu.wave_channels[channel].is_playing = false;
            }
            break;
        case GB_IO_NR12:
        case GB_IO_NR22:
        case GB_IO_NR42:
            gb->apu.wave_channels[channel].start_amplitude =
            gb->apu.wave_channels[channel].amplitude = CH_STEP * (value >> 4);
            if (value >> 4 == 0) {
                gb->apu.wave_channels[channel].is_playing = false;
            }
            gb->apu.wave_channels[channel].envelope_direction = value & 8? 1 : -1;
            gb->apu.wave_channels[channel].cur_envelope_steps =
            gb->apu.wave_channels[channel].envelope_steps = value & 7;
            break;
        case GB_IO_NR13:
        case GB_IO_NR23:
        case GB_IO_NR33:
            NRX3_X4_temp[channel] = (NRX3_X4_temp[channel] & 0xFF00) | value;
            gb->apu.wave_channels[channel].frequency =  131072.0 / (2048 - NRX3_X4_temp[channel]);
            if (channel == 2) {
                gb->apu.wave_channels[channel].frequency /= 2;
            }
            break;
        case GB_IO_NR14:
        case GB_IO_NR24:
        case GB_IO_NR34:
            gb->apu.wave_channels[channel].stop_on_length = value & 0x40;
            if (value & 0x80) {
                gb->apu.wave_channels[channel].is_playing = true;
                gb->apu.wave_channels[channel].phase = 0;
                gb->apu.wave_channels[channel].amplitude = gb->apu.wave_channels[channel].start_amplitude;
                gb->apu.wave_channels[channel].cur_envelope_steps = gb->apu.wave_channels[channel].envelope_steps;
            }

            NRX3_X4_temp[channel] = (NRX3_X4_temp[channel] & 0xFF) | ((value & 0x7) << 8);
            gb->apu.wave_channels[channel].frequency =  131072.0 / (2048 - NRX3_X4_temp[channel]);
            if (channel == 2) {
                gb->apu.wave_channels[channel].frequency /= 2;
            }
            break;
        case GB_IO_NR30:
            gb->apu.wave_enable = value & 0x80;
            break;
        case GB_IO_NR31:
            gb->apu.wave_channels[2].sound_length = (256 - value) / 256.0;
            if (gb->apu.wave_channels[2].sound_length == 0) {
                gb->apu.wave_channels[2].is_playing = false;
            }
            break;
        case GB_IO_NR32:
            gb->apu.wave_shift = ((value >> 5) + 3) & 3;
            if (gb->apu.wave_shift == 3) {
                gb->apu.wave_shift = 4;
            }
            break;
        case GB_IO_NR43:
        {
            double r = value & 0x7;
            if (r == 0) r = 0.5;
            unsigned char s = value >> 4;
            gb->apu.wave_channels[3].frequency =  524288.0 / r / (1 << (s + 1));
            gb->apu.lfsr_7_bit = value & 0x8;
            break;
        }
        case GB_IO_NR44:
            gb->apu.wave_channels[3].stop_on_length = value & 0x40;
            if (value & 0x80) {
                gb->apu.wave_channels[3].is_playing = true;
                gb->apu.lfsr = 0x7FFF;
                gb->apu.wave_channels[3].amplitude = gb->apu.wave_channels[3].start_amplitude;
                gb->apu.wave_channels[3].cur_envelope_steps = gb->apu.wave_channels[3].envelope_steps;
            }
            break;

        case GB_IO_NR50:
            gb->apu.left_volume = (value & 7) / 7.0;
            gb->apu.right_volume = ((value >> 4) & 7) / 7.0;
            break;

        case GB_IO_NR51:
            for (int i = 0; i < 4; i++) {
                gb->apu.left_on[i] = value & 1;
                gb->apu.right_on[i] = value & 0x10;
                value >>= 1;
            }
            break;
        case GB_IO_NR52:

            if ((value & 0x80) && !gb->apu.global_enable) {
                apu_init(gb);
                gb->apu.global_enable = true;
            }
            else if (!(value & 0x80) && gb->apu.global_enable)  {
                memset(&gb->apu, 0, sizeof(gb->apu));
                memset(gb->io_registers + GB_IO_NR10, 0, GB_IO_WAV_START - GB_IO_NR10);
            }
            break;

        default:
            if (reg >= GB_IO_WAV_START && reg <= GB_IO_WAV_END) {
                gb->apu.wave_form[(reg - GB_IO_WAV_START) * 2] = value >> 4;
                gb->apu.wave_form[(reg - GB_IO_WAV_START) * 2 + 1] = value & 0xF;
            }
            break;
    }
}