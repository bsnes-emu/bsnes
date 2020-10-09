#include "gb.h"
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#else
#include <sys/time.h>
#endif

static const unsigned GB_TAC_TRIGGER_BITS[] = {512, 8, 32, 128};

#ifndef GB_DISABLE_TIMEKEEPING
static int64_t get_nanoseconds(void)
{
#ifndef _WIN32
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;
#else
    FILETIME time;
    GetSystemTimeAsFileTime(&time);
    return (((int64_t)time.dwHighDateTime << 32) | time.dwLowDateTime) * 100L;
#endif
}

static void nsleep(uint64_t nanoseconds)
{
#ifndef _WIN32
    struct timespec sleep = {0, nanoseconds};
    nanosleep(&sleep, NULL);
#else
    HANDLE timer;
    LARGE_INTEGER time;
    timer = CreateWaitableTimer(NULL, true, NULL);
    time.QuadPart = -(nanoseconds / 100L);
    SetWaitableTimer(timer, &time, 0, NULL, NULL, false);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
#endif
}

bool GB_timing_sync_turbo(GB_gameboy_t *gb)
{
    if (!gb->turbo_dont_skip) {
        int64_t nanoseconds = get_nanoseconds();
        if (nanoseconds <= gb->last_sync + (1000000000LL * LCDC_PERIOD / GB_get_clock_rate(gb))) {
            return true;
        }
        gb->last_sync = nanoseconds;
    }
    return false;
}

void GB_timing_sync(GB_gameboy_t *gb)
{
    if (gb->turbo) {
        gb->cycles_since_last_sync = 0;
        return;
    }
    /* Prevent syncing if not enough time has passed.*/
    if (gb->cycles_since_last_sync < LCDC_PERIOD / 3) return;

    uint64_t target_nanoseconds = gb->cycles_since_last_sync * 1000000000LL / 2 / GB_get_clock_rate(gb); /* / 2 because we use 8MHz units */
    int64_t nanoseconds = get_nanoseconds();
    int64_t time_to_sleep = target_nanoseconds + gb->last_sync - nanoseconds;
    if (time_to_sleep > 0 && time_to_sleep < LCDC_PERIOD * 1000000000LL / GB_get_clock_rate(gb)) {
        nsleep(time_to_sleep);
        gb->last_sync += target_nanoseconds;
    }
    else {
        gb->last_sync = nanoseconds;
    }

    gb->cycles_since_last_sync = 0;
    if (gb->update_input_hint_callback) {
        gb->update_input_hint_callback(gb);
    }
}
#else

bool GB_timing_sync_turbo(GB_gameboy_t *gb)
{
    return false;
}

void GB_timing_sync(GB_gameboy_t *gb)
{
}

#endif
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
        gb->io_registers[GB_IO_IF] |= 4;
        gb->tima_reload_state = GB_TIMA_RELOADED;
    }
}

static void increase_tima(GB_gameboy_t *gb)
{
    gb->io_registers[GB_IO_TIMA]++;
    if (gb->io_registers[GB_IO_TIMA] == 0) {
        gb->io_registers[GB_IO_TIMA] = gb->io_registers[GB_IO_TMA];
        gb->tima_reload_state = GB_TIMA_RELOADING;
    }
}

static void GB_set_internal_div_counter(GB_gameboy_t *gb, uint32_t value)
{
    /* TIMA increases when a specific high-bit becomes a low-bit. */
    value &= INTERNAL_DIV_CYCLES - 1;
    uint32_t triggers = gb->div_counter & ~value;
    if ((gb->io_registers[GB_IO_TAC] & 4) && (triggers & GB_TAC_TRIGGER_BITS[gb->io_registers[GB_IO_TAC] & 3])) {
        increase_tima(gb);
    }
    
    /* TODO: Can switching to double speed mode trigger an event? */
    if (triggers & (gb->cgb_double_speed? 0x2000 : 0x1000)) {
        GB_apu_run(gb);
        GB_apu_div_event(gb);
    }
    gb->div_counter = value;
}

static void GB_timers_run(GB_gameboy_t *gb, uint8_t cycles)
{
    if (gb->stopped) {
        gb->apu.apu_cycles += 4 << !gb->cgb_double_speed;
        return;
    }
    
    GB_STATE_MACHINE(gb, div, cycles, 1) {
        GB_STATE(gb, div, 1);
        GB_STATE(gb, div, 2);
        GB_STATE(gb, div, 3);
    }
    
    GB_set_internal_div_counter(gb, 0);
main:
    GB_SLEEP(gb, div, 1, 3);
    while (true) {
        advance_tima_state_machine(gb);
        GB_set_internal_div_counter(gb, gb->div_counter + 4);
        gb->apu.apu_cycles += 4 << !gb->cgb_double_speed;
        GB_SLEEP(gb, div, 2, 4);
    }
    
    /* Todo: This is ugly to allow compatibility with 0.11 save states. Fix me when breaking save compatibility */
    {
        div3:
        /* Compensate for lack of prefetch emulation, as well as DIV's internal initial value */
        GB_set_internal_div_counter(gb, 8);
        goto main;
    }
}

static void advance_serial(GB_gameboy_t *gb, uint8_t cycles)
{
    if (gb->serial_length == 0) {
        gb->serial_cycles += cycles;
        return;
    }
    
    while (cycles > gb->serial_length) {
        advance_serial(gb, gb->serial_length);
        cycles -= gb->serial_length;
    }
    
    uint16_t previous_serial_cycles = gb->serial_cycles;
    gb->serial_cycles += cycles;
    if ((gb->serial_cycles & gb->serial_length) != (previous_serial_cycles & gb->serial_length)) {
        gb->serial_count++;
        if (gb->serial_count == 8) {
            gb->serial_length = 0;
            gb->serial_count = 0;
            gb->io_registers[GB_IO_SC] &= ~0x80;
            gb->io_registers[GB_IO_IF] |= 8;
        }
        
        gb->io_registers[GB_IO_SB] <<= 1;
        
        if (gb->serial_transfer_bit_end_callback) {
            gb->io_registers[GB_IO_SB] |= gb->serial_transfer_bit_end_callback(gb);
        }
        else {
            gb->io_registers[GB_IO_SB] |= 1;
        }
        
        if (gb->serial_length) {
            /* Still more bits to send */
            if (gb->serial_transfer_bit_start_callback) {
                gb->serial_transfer_bit_start_callback(gb, gb->io_registers[GB_IO_SB] & 0x80);
            }
        }
        
    }
    return;
    
}

void GB_advance_cycles(GB_gameboy_t *gb, uint8_t cycles)
{
    gb->apu.pcm_mask[0] = gb->apu.pcm_mask[1] = 0xFF; // Sort of hacky, but too many cross-component interactions to do it right
    // Affected by speed boost
    gb->dma_cycles += cycles;

    GB_timers_run(gb, cycles);
    if (!gb->stopped) {
        advance_serial(gb, cycles); // TODO: Verify what happens in STOP mode
    }

    gb->debugger_ticks += cycles;

    if (!gb->cgb_double_speed) {
        cycles <<= 1;
    }
    
    // Not affected by speed boost
    gb->double_speed_alignment += cycles;
    gb->hdma_cycles += cycles;
    gb->apu_output.sample_cycles += cycles;
    gb->cycles_since_ir_change += cycles;
    gb->cycles_since_input_ir_change += cycles;
    gb->cycles_since_last_sync += cycles;
    gb->cycles_since_run += cycles;
    
    if (gb->rumble_state) {
        gb->rumble_on_cycles++;
    }
    else {
        gb->rumble_off_cycles++;
    }
    
    if (!gb->stopped) { // TODO: Verify what happens in STOP mode
        GB_dma_run(gb);
        GB_hdma_run(gb);
    }
    GB_apu_run(gb);
    GB_display_run(gb, cycles);
    GB_ir_run(gb);
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

    unsigned old_clocks = GB_TAC_TRIGGER_BITS[old_tac & 3];
    unsigned new_clocks = GB_TAC_TRIGGER_BITS[new_tac & 3];

    /* The bit used for overflow testing must have been 1 */
    if (gb->div_counter & old_clocks) {
        /* And now either the timer must be disabled, or the new bit used for overflow testing be 0. */
        if (!(new_tac & 4) || gb->div_counter & new_clocks) {
            increase_tima(gb);
        }
    }
}

void GB_rtc_run(GB_gameboy_t *gb)
{
    if (gb->cartridge_type->mbc_type == GB_HUC3) {
        time_t current_time = time(NULL);
        while (gb->last_rtc_second / 60 < current_time / 60) {
            gb->last_rtc_second += 60;
            gb->huc3_minutes++;
            if (gb->huc3_minutes == 60 * 24) {
                gb->huc3_days++;
                gb->huc3_minutes = 0;
            }
        }
        return;
    }
    
    if ((gb->rtc_real.high & 0x40) == 0) { /* is timer running? */
        time_t current_time = time(NULL);
        
        while (gb->last_rtc_second + 60 * 60 * 24 < current_time) {
            gb->last_rtc_second += 60 * 60 * 24;
            if (++gb->rtc_real.days == 0) {
                if (gb->rtc_real.high & 1) { /* Bit 8 of days*/
                    gb->rtc_real.high |= 0x80; /* Overflow bit */
                }
                gb->rtc_real.high ^= 1;
            }
        }
        
        while (gb->last_rtc_second < current_time) {
            gb->last_rtc_second++;
            if (++gb->rtc_real.seconds == 60) { 
                gb->rtc_real.seconds = 0;
                if (++gb->rtc_real.minutes == 60) { 
                    gb->rtc_real.minutes = 0;
                    if (++gb->rtc_real.hours == 24) { 
                        gb->rtc_real.hours = 0;
                        if (++gb->rtc_real.days == 0) { 
                            if (gb->rtc_real.high & 1) { /* Bit 8 of days*/
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
