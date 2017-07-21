#include <stdint.h>
#include <math.h>
#include <string.h>
#include "gb.h"

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

static void update_sample(GB_gameboy_t *gb, unsigned index, uint8_t value)
{
    gb->apu.samples[index] = value;
}

void GB_apu_div_event(GB_gameboy_t *gb)
{
    if (gb->apu.is_active[GB_WAVE] && gb->apu.wave_channel.length_enabled) {
        if (gb->apu.wave_channel.pulse_length) {
            gb->apu.wave_channel.pulse_length--;
        }
        else {
            gb->apu.is_active[GB_WAVE] = false;
            gb->apu.wave_channel.current_sample = 0;
            update_sample(gb, GB_WAVE, 0);
        }
    }
}

static void render(GB_gameboy_t *gb)
{
    while (gb->audio_copy_in_progress);
    while (!__sync_bool_compare_and_swap(&gb->apu_lock, false, true));
    if (gb->audio_position >= gb->buffer_size) {
        gb->apu_lock = false;
        return;
    }
    gb->audio_buffer[gb->audio_position++] = (GB_sample_t) {gb->apu.samples[GB_WAVE] * CH_STEP,
                                                            gb->apu.samples[GB_WAVE] * CH_STEP};
    gb->apu_lock = false;
}

void GB_apu_run(GB_gameboy_t *gb, uint8_t cycles)
{
    /* Convert 4MHZ to 2MHz. cycles is always even. */
    cycles >>= 1;
    
    double cycles_per_sample = gb->sample_rate ? CPU_FREQUENCY / (double)gb->sample_rate : 0; // TODO: this should be cached!
    
    if (gb->sample_rate && gb->apu_sample_cycles > cycles_per_sample) {
        gb->apu_sample_cycles -= cycles_per_sample;
        render(gb);
    }
    
    gb->apu.wave_channel.wave_form_just_read = false;
    if (gb->apu.is_active[GB_WAVE]) {
        uint8_t cycles_left = cycles;
        while (unlikely(cycles_left > gb->apu.wave_channel.sample_countdown)) {
            cycles_left -= gb->apu.wave_channel.sample_countdown + 1;
            gb->apu.wave_channel.sample_countdown = gb->apu.wave_channel.sample_length;
            gb->apu.wave_channel.current_sample_index++;
            gb->apu.wave_channel.current_sample_index &= 0x1F;
            gb->apu.wave_channel.current_sample =
                gb->apu.wave_channel.wave_form[gb->apu.wave_channel.current_sample_index];
            update_sample(gb, GB_WAVE, gb->apu.wave_channel.current_sample >> gb->apu.wave_channel.shift);
            gb->apu.wave_channel.wave_form_just_read = true;
        }
        if (cycles_left) {
            gb->apu.wave_channel.sample_countdown -= cycles_left;
            gb->apu.wave_channel.wave_form_just_read = false;
        }
    }
    
}

void GB_apu_copy_buffer(GB_gameboy_t *gb, GB_sample_t *dest, unsigned int count)
{
    gb->audio_copy_in_progress = true;

    if (!gb->audio_stream_started) {
        // Intentionally fail the first copy to sync the stream with the Gameboy.
        gb->audio_stream_started = true;
        gb->audio_position = 0;
    }

    if (count > gb->audio_position) {
        // GB_log(gb, "Audio underflow: %d\n", count - gb->audio_position);
        if (gb->audio_position != 0) {
            for (unsigned i = 0; i < count - gb->audio_position; i++) {
                dest[gb->audio_position + i] = gb->audio_buffer[gb->audio_position - 1];
            }
        }
        else {
            memset(dest + gb->audio_position, 0, (count - gb->audio_position) * sizeof(*gb->audio_buffer));
        }
        count = gb->audio_position;
    }
    memcpy(dest, gb->audio_buffer, count * sizeof(*gb->audio_buffer));
    memmove(gb->audio_buffer, gb->audio_buffer + count, (gb->audio_position - count) * sizeof(*gb->audio_buffer));
    gb->audio_position -= count;

    gb->audio_copy_in_progress = false;
}

void GB_apu_init(GB_gameboy_t *gb)
{
    memset(&gb->apu, 0, sizeof(gb->apu));
    // gb->apu.wave_channels[0].duty = gb->apu.wave_channels[1].duty = 4;
    // gb->apu.lfsr = 0x7FFF;
    gb->apu.left_volume = 7;
    gb->apu.right_volume = 7;
    for (int i = 0; i < 4; i++) {
        gb->apu.left_enabled[i] = gb->apu.right_enabled[i] = true;
    }
    gb->apu.wave_channel.sample_length = 0x7FF;
}

uint8_t GB_apu_read(GB_gameboy_t *gb, uint8_t reg)
{
    if (reg == GB_IO_NR52) {
        uint8_t value = 0;
        for (int i = 0; i < GB_N_CHANNELS; i++) {
            value >>= 1;
            if (gb->apu.is_active[i]) {
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

    if (reg >= GB_IO_WAV_START && reg <= GB_IO_WAV_END && gb->apu.is_active[GB_WAVE]) {
        if (!gb->is_cgb && !gb->apu.wave_channel.wave_form_just_read) {
            return 0xFF;
        }
        reg = GB_IO_WAV_START + gb->apu.wave_channel.current_sample_index / 2;
    }

    return gb->io_registers[reg] | read_mask[reg - GB_IO_NR10];
}

void GB_apu_write(GB_gameboy_t *gb, uint8_t reg, uint8_t value)
{
    if (!gb->apu.global_enable && reg != GB_IO_NR52) {
        return;
    }

    if (reg >= GB_IO_WAV_START && reg <= GB_IO_WAV_END && gb->apu.is_active[GB_WAVE]) {
        if (!gb->is_cgb && !gb->apu.wave_channel.wave_form_just_read) {
            return;
        }
        reg = GB_IO_WAV_START + gb->apu.wave_channel.current_sample_index / 2;
    }
    
    gb->io_registers[reg] = value;


    switch (reg) {
        /* Globals */
        case GB_IO_NR52:
            if ((value & 0x80) && !gb->apu.global_enable) {
                GB_apu_init(gb);
                gb->apu.global_enable = true;
            }
            else if (!(value & 0x80) && gb->apu.global_enable)  {
                memset(&gb->apu, 0, sizeof(gb->apu));
                memset(gb->io_registers + GB_IO_NR10, 0, GB_IO_WAV_START - GB_IO_NR10);
                gb->apu.global_enable = false;
            }
            break;
            
        /* Wave channel */
        case GB_IO_NR30:
            gb->apu.wave_channel.enable = value & 0x80;
            if (!gb->apu.wave_channel.enable) {
                gb->apu.is_active[GB_WAVE] = false;
                gb->apu.wave_channel.current_sample = 0;
                update_sample(gb, GB_WAVE, 0);
            }
            break;
        case GB_IO_NR31:
            break;
        case GB_IO_NR32:
            gb->apu.wave_channel.shift = (uint8_t[]){4, 0, 1, 2}[(value >> 5) & 3];
            update_sample(gb, GB_WAVE, gb->apu.wave_channel.current_sample >> gb->apu.wave_channel.shift);
            break;
        case GB_IO_NR33:
            gb->apu.wave_channel.sample_length &= ~0xFF;
            gb->apu.wave_channel.sample_length |= (~value) & 0xFF;
            break;
        case GB_IO_NR34:
            gb->apu.wave_channel.length_enabled = value & 0x40;
            gb->apu.wave_channel.sample_length &= 0xFF;
            gb->apu.wave_channel.sample_length |= ((~value) & 7) << 8;
            if ((value & 0x80) && gb->apu.wave_channel.enable) {
                /* DMG bug: wave RAM gets corrupted if the channel is retriggerred 1 cycle before the APU
                            reads from it. */
                if (!gb->is_cgb && gb->apu.is_active[GB_WAVE] && gb->apu.wave_channel.sample_countdown == 0) {
                    unsigned offset = ((gb->apu.wave_channel.current_sample_index + 1) >> 1) & 0xF;
                    
                    /* On SGB2 (and probably SGB1 and MGB as well) this behavior is not accurate,
                       however these systems are not currently emulated. */
                    if (offset < 4) {
                        gb->io_registers[GB_IO_WAV_START] = gb->io_registers[GB_IO_WAV_START + offset];
                        gb->apu.wave_channel.wave_form[0] = gb->apu.wave_channel.wave_form[offset / 2];
                        gb->apu.wave_channel.wave_form[1] = gb->apu.wave_channel.wave_form[offset / 2 + 1];
                    }
                    else {
                        memcpy(gb->io_registers + GB_IO_WAV_START,
                               gb->io_registers + GB_IO_WAV_START + (offset & ~3),
                               4);
                        memcpy(gb->apu.wave_channel.wave_form,
                               gb->apu.wave_channel.wave_form + (offset & ~3) * 2,
                               8);
                    }
                }
                gb->apu.is_active[GB_WAVE] = true;
                gb->apu.wave_channel.pulse_length = ~gb->io_registers[GB_IO_NR31];
                gb->apu.wave_channel.sample_countdown = gb->apu.wave_channel.sample_length + 3;
                gb->apu.wave_channel.current_sample_index = 0;
                /* Note that we don't change the sample just yet! This was verified on hardware. */
            }
            break;
        
        default:
            if (reg >= GB_IO_WAV_START && reg <= GB_IO_WAV_END) {
                gb->apu.wave_channel.wave_form[(reg - GB_IO_WAV_START) * 2]     = value >> 4;
                gb->apu.wave_channel.wave_form[(reg - GB_IO_WAV_START) * 2 + 1] = value & 0xF;
            }
    }


}

unsigned GB_apu_get_current_buffer_length(GB_gameboy_t *gb)
{
    return  gb->audio_position;
}
