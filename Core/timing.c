#include "gb.h"
#include "timing.h"
#include "memory.h"
#include "display.h"

void GB_advance_cycles(GB_gameboy_t *gb, uint8_t cycles)
{
    // Affected by speed boost
    if (gb->dma_cycles > cycles){
        gb->dma_cycles -= cycles;
    }
    else {
        gb->dma_cycles = 0;
    }

    gb->div_cycles += cycles;
    gb->tima_cycles += cycles;

    if (gb->cgb_double_speed) {
        cycles >>=1;
    }

    // Not affected by speed boost
    gb->hdma_cycles += cycles;
    gb->display_cycles += cycles;
    gb->apu_cycles += cycles;
    GB_hdma_run(gb);
    GB_timers_run(gb);
    GB_apu_run(gb);
    GB_display_run(gb);
}

void GB_timers_run(GB_gameboy_t *gb)
{
    /* Standard Timers */
    static const unsigned int GB_TAC_RATIOS[] = {1024, 16, 64, 256};

    if (gb->div_cycles >= DIV_CYCLES) {
        gb->div_cycles -= DIV_CYCLES;
        gb->io_registers[GB_IO_DIV]++;
    }

    while (gb->tima_cycles >= GB_TAC_RATIOS[gb->io_registers[GB_IO_TAC] & 3]) {
        gb->tima_cycles -= GB_TAC_RATIOS[gb->io_registers[GB_IO_TAC] & 3];
        if (gb->io_registers[GB_IO_TAC] & 4) {
            gb->io_registers[GB_IO_TIMA]++;
            if (gb->io_registers[GB_IO_TIMA] == 0) {
                gb->io_registers[GB_IO_TIMA] = gb->io_registers[GB_IO_TMA];
                gb->io_registers[GB_IO_IF] |= 4;
            }
        }
    }
}

void GB_rtc_run(GB_gameboy_t *gb)
{
    if ((gb->rtc_high & 0x40) == 0) { /* is timer running? */
        time_t current_time = time(NULL);
        while (gb->last_rtc_second < current_time) {
            gb->last_rtc_second++;
            if (++gb->rtc_seconds == 60)
            {
                gb->rtc_seconds = 0;
                if (++gb->rtc_minutes == 60)
                {
                    gb->rtc_minutes = 0;
                    if (++gb->rtc_hours == 24)
                    {
                        gb->rtc_hours = 0;
                        if (++gb->rtc_days == 0)
                        {
                            if (gb->rtc_high & 1) /* Bit 8 of days*/
                            {
                                gb->rtc_high |= 0x80; /* Overflow bit */
                            }
                            gb->rtc_high ^= 1;
                        }
                    }
                }
            }
        }
    }
}