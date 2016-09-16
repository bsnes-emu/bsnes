#include "gb.h"
#include "timing.h"
#include "memory.h"
#include "display.h"

static void GB_ir_run(GB_gameboy_t *gb)
{
    if (gb->ir_queue_length == 0) return;
    if (gb->cycles_since_input_ir_change >= gb->ir_queue[0].delay) {
        gb->cycles_since_input_ir_change -= gb->ir_queue[0].delay;
        gb->infrared_input = gb->ir_queue[0].state;
        gb->ir_queue_length--;
        memmove(&gb->ir_queue[0], &gb->ir_queue[1], sizeof(gb->ir_queue[0]) * (gb->ir_queue_length));
    }
}

static void advance_tima_state_machine(GB_gameboy_t *gb)
{
    if (gb->tima_reload_state == GB_TIMA_RELOADED) {
        gb->tima_reload_state = GB_TIMA_RUNNING;
    }
    else if (gb->tima_reload_state == GB_TIMA_RELOADING) {
        gb->tima_reload_state = GB_TIMA_RELOADED;
    }
}

void GB_advance_cycles(GB_gameboy_t *gb, uint8_t cycles)
{
    // Affected by speed boost
    gb->dma_cycles += cycles;

    advance_tima_state_machine(gb);
    for (int i = 0; i < cycles; i += 4) {
        GB_set_internal_div_counter(gb, gb->div_cycles + 4);
    }

    if (cycles > 4) {
        advance_tima_state_machine(gb);
        if (cycles > 8) {
            advance_tima_state_machine(gb);
        }
    }

    if (gb->serial_cycles) {
        if (gb->serial_cycles <= cycles) {
            gb->serial_cycles = 0;
            gb->io_registers[GB_IO_SC] &= ~0x80;
            gb->io_registers[GB_IO_SB] = 0xFF;
            gb->io_registers[GB_IO_IF] |= 8;
        }
        else {
            gb->serial_cycles -= cycles;
        }
    }

    gb->debugger_ticks += cycles;

    if (gb->cgb_double_speed) {
        cycles >>=1;
    }

    // Not affected by speed boost
    gb->hdma_cycles += cycles;
    gb->display_cycles += cycles;
    gb->apu_cycles += cycles;
    gb->cycles_since_ir_change += cycles;
    gb->cycles_since_input_ir_change += cycles;
    GB_dma_run(gb);
    GB_hdma_run(gb);
    GB_apu_run(gb);
    GB_display_run(gb);
    GB_ir_run(gb);
}

/* Standard Timers */
static const unsigned int GB_TAC_RATIOS[] = {1024, 16, 64, 256};

static void increase_tima(GB_gameboy_t *gb)
{
    gb->io_registers[GB_IO_TIMA]++;
    if (gb->io_registers[GB_IO_TIMA] == 0) {
        gb->io_registers[GB_IO_TIMA] = gb->io_registers[GB_IO_TMA];
        gb->io_registers[GB_IO_IF] |= 4;
        gb->tima_reload_state = GB_TIMA_RELOADING;
    }
}

static bool counter_overflow_check(uint32_t old, uint32_t new, uint32_t max)
{
    return (old & (max >> 1)) && !(new & (max >> 1));
}

void GB_set_internal_div_counter(GB_gameboy_t *gb, uint32_t value)
{
    /* DIV and TIMA increase when a specific high-bit becomes a low-bit. */
    value &= INTERNAL_DIV_CYCLES - 1;
    if (counter_overflow_check(gb->div_cycles, value, DIV_CYCLES)) {
        gb->io_registers[GB_IO_DIV]++;
    }
    if ((gb->io_registers[GB_IO_TAC] & 4) &&
        counter_overflow_check(gb->div_cycles, value, GB_TAC_RATIOS[gb->io_registers[GB_IO_TAC] & 3])) {
        increase_tima(gb);
    }
    gb->div_cycles = value;
}

/* 
   This glitch is based on the expected results of mooneye-gb rapid_toggle test.
   This glitch happens because how TIMA is increased, see GB_set_internal_div_counter.
   According to GiiBiiAdvance, GBC's behavior is different, but this was not tested or implemented.
*/
void GB_emulate_timer_glitch(GB_gameboy_t *gb, uint8_t old_tac, uint8_t new_tac)
{
    /* Glitch only happens when old_tac is enabled. */
    if (!(old_tac & 4)) return;

    unsigned int old_clocks = GB_TAC_RATIOS[old_tac & 3];
    unsigned int new_clocks = GB_TAC_RATIOS[new_tac & 3];

    /* The bit used for overflow testing must have been 1 */
    if (gb->div_cycles & (old_clocks >> 1)) {
        /* And now either the timer must be disabled, or the new bit used for overflow testing be 0. */
        if (!(new_tac & 4) || gb->div_cycles & (new_clocks >> 1)) {
            increase_tima(gb);
        }
    }
}

void GB_rtc_run(GB_gameboy_t *gb)
{
    if ((gb->rtc_real.high & 0x40) == 0) { /* is timer running? */
        time_t current_time = time(NULL);
        while (gb->last_rtc_second < current_time) {
            gb->last_rtc_second++;
            if (++gb->rtc_real.seconds == 60)
            {
                gb->rtc_real.seconds = 0;
                if (++gb->rtc_real.minutes == 60)
                {
                    gb->rtc_real.minutes = 0;
                    if (++gb->rtc_real.hours == 24)
                    {
                        gb->rtc_real.hours = 0;
                        if (++gb->rtc_real.days == 0)
                        {
                            if (gb->rtc_real.high & 1) /* Bit 8 of days*/
                            {
                                gb->rtc_real.high |= 0x80; /* Overflow bit */
                            }
                            gb->rtc_real.high ^= 1;
                        }
                    }
                }
            }
        }
    }
}