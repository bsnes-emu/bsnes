#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/time.h>
#include "gb.h"
#include "memory.h"
#include "timing.h"
#include "z80_cpu.h"
#include "joypad.h"
#include "display.h"
#include "debugger.h"

static const GB_cartridge_t cart_defs[256] = {
    // From http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header#0147_-_Cartridge_Type
    /* MBC     RAM    BAT.   RTC    RUMB.    */
    {  NO_MBC, false, false, false, false}, // 00h  ROM ONLY
    {  MBC1  , false, false, false, false}, // 01h  MBC1
    {  MBC1  , true , false, false, false}, // 02h  MBC1+RAM
    {  MBC1  , true , true , false, false}, // 03h  MBC1+RAM+BATTERY
    [5] =
    {  MBC2  , true , false, false, false}, // 05h  MBC2
    {  MBC2  , true , true , false, false}, // 06h  MBC2+BATTERY
    [8] =
    {  NO_MBC, true , false, false, false}, // 08h  ROM+RAM
    {  NO_MBC, true , true , false, false}, // 09h  ROM+RAM+BATTERY
    [0xB] =
    // Todo: What are these?
    {  NO_MBC, false, false, false, false}, // 0Bh  MMM01
    {  NO_MBC, false, false, false, false}, // 0Ch  MMM01+RAM
    {  NO_MBC, false, false, false, false}, // 0Dh  MMM01+RAM+BATTERY
    [0xF] =
    {  MBC3  , false, true,  true , false}, // 0Fh  MBC3+TIMER+BATTERY
    {  MBC3  , true , true,  true , false}, // 10h  MBC3+TIMER+RAM+BATTERY
    {  MBC3  , false, false, false, false}, // 11h  MBC3
    {  MBC3  , true , false, false, false}, // 12h  MBC3+RAM
    {  MBC3  , true , true , false, false}, // 13h  MBC3+RAM+BATTERY
    [0x15] =
    // Todo: Do these exist?
    {  MBC4  , false, false, false, false}, // 15h  MBC4
    {  MBC4  , true , false, false, false}, // 16h  MBC4+RAM
    {  MBC4  , true , true , false, false}, // 17h  MBC4+RAM+BATTERY
    [0x19] =
    {  MBC5  , false, false, false, false}, // 19h  MBC5
    {  MBC5  , true , false, false, false}, // 1Ah  MBC5+RAM
    {  MBC5  , true , true , false, false}, // 1Bh  MBC5+RAM+BATTERY
    {  MBC5  , false, false, false, true }, // 1Ch  MBC5+RUMBLE
    {  MBC5  , true , false, false, true }, // 1Dh  MBC5+RUMBLE+RAM
    {  MBC5  , true , true , false, true }, // 1Eh  MBC5+RUMBLE+RAM+BATTERY
    [0xFC] =
    // Todo: What are these?
    {  NO_MBC, false, false, false, false}, // FCh  POCKET CAMERA
    {  NO_MBC, false, false, false, false}, // FDh  BANDAI TAMA5
    {  NO_MBC, false, false, false, false}, // FEh  HuC3
    {  NO_MBC, true , true , false, false}, // FFh  HuC1+RAM+BATTERY
};

void gb_attributed_logv(GB_gameboy_t *gb, gb_log_attributes attributes, const char *fmt, va_list args)
{
    char *string = NULL;
    vasprintf(&string, fmt, args);
    if (string) {
        if (gb->log_callback) {
            gb->log_callback(gb, string, attributes);
        }
        else {
            /* Todo: Add ANSI escape sequences for attributed text */
            printf("%s", string);
        }
    }
    free(string);
}

void gb_attributed_log(GB_gameboy_t *gb, gb_log_attributes attributes, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    gb_attributed_logv(gb, attributes, fmt, args);
    va_end(args);
}

void gb_log(GB_gameboy_t *gb,const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    gb_attributed_logv(gb, 0, fmt, args);
    va_end(args);
}

static char *default_input_callback(GB_gameboy_t *gb)
{
    char *expression = NULL;
    size_t size = 0;
    printf(">");
    getline(&expression, &size, stdin);

    if (!expression) {
        return strdup("");
    }

    size_t length = strlen(expression);
    if (expression[length - 1] == '\n') {
        expression[length - 1] = 0;
    }
    return expression;
}

void gb_init(GB_gameboy_t *gb)
{
    memset(gb, 0, sizeof(*gb));
    gb->magic = (uintptr_t)'SAME';
    gb->version = GB_STRUCT_VERSION;
    gb->ram = malloc(gb->ram_size = 0x2000);
    gb->vram = malloc(gb->vram_size = 0x2000);

    struct timeval now;
    gettimeofday(&now, NULL);
    gb->last_vblank = (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;;

    gb->mbc_rom_bank = 1;
    gb->last_rtc_second = time(NULL);
    gb->last_vblank = clock();
    gb->cgb_ram_bank = 1;

    /* Todo: this bypasses the rgb encoder because it is not set yet. */
    gb->sprite_palletes_rgb[4] = gb->sprite_palletes_rgb[0] = gb->background_palletes_rgb[0] = 0xFFFFFFFF;
    gb->sprite_palletes_rgb[5] = gb->sprite_palletes_rgb[1] = gb->background_palletes_rgb[1] = 0xAAAAAAAA;
    gb->sprite_palletes_rgb[6] = gb->sprite_palletes_rgb[2] = gb->background_palletes_rgb[2] = 0x55555555;
    gb->input_callback = default_input_callback;
    gb->cartridge_type = &cart_defs[0]; // Default cartridge type
}

void gb_init_cgb(GB_gameboy_t *gb)
{
    memset(gb, 0, sizeof(*gb));
    gb->magic = (uintptr_t)'SAME';
    gb->version = GB_STRUCT_VERSION;
    gb->ram = malloc(gb->ram_size = 0x2000 * 8);
    gb->vram = malloc(gb->vram_size = 0x2000 * 2);
    gb->is_cgb = true;
    gb->cgb_mode = true;

    struct timeval now;
    gettimeofday(&now, NULL);
    gb->last_vblank = (now.tv_usec) * 1000 + now.tv_sec * 1000000000L;

    gb->mbc_rom_bank = 1;
    gb->last_rtc_second = time(NULL);
    gb->last_vblank = clock();
    gb->cgb_ram_bank = 1;
    gb->input_callback = default_input_callback;
    gb->cartridge_type = &cart_defs[0]; // Default cartridge type
}

void gb_free(GB_gameboy_t *gb)
{
    if (gb->ram) {
        free(gb->ram);
    }
    if (gb->vram) {
        free(gb->vram);
    }
    if (gb->mbc_ram) {
        free(gb->mbc_ram);
    }
    if (gb->rom) {
        free(gb->rom);
    }
    if (gb->audio_buffer) {
        free(gb->audio_buffer);
    }
    if (gb->breakpoints) {
        free(gb->breakpoints);
    }
}

int gb_load_bios(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return errno;
    fread(gb->bios, sizeof(gb->bios), 1, f);
    fclose(f);
    return 0;
}

int gb_load_rom(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return errno;
    fseek(f, 0, SEEK_END);
    gb->rom_size = (ftell(f) + 0x3FFF) & ~0x3FFF; /* Round to bank */
    fseek(f, 0, SEEK_SET);
    gb->rom = malloc(gb->rom_size);
    memset(gb->rom, 0xFF, gb->rom_size); /* Pad with 0xFFs */
    fread(gb->rom, gb->rom_size, 1, f);
    fclose(f);
    gb->cartridge_type = &cart_defs[gb->rom[0x147]];
    if (gb->cartridge_type->has_ram) {
        static const int ram_sizes[256] = {0, 0x800, 0x2000, 0x8000, 0x20000, 0x10000};
        gb->mbc_ram_size = ram_sizes[gb->rom[0x149]];
        gb->mbc_ram = malloc(gb->mbc_ram_size);
    }

    return 0;
}

/* Todo: we need a sane and protable save state format. */
int gb_save_state(GB_gameboy_t *gb, const char *path)
{
    GB_gameboy_t save;
    memcpy(&save, gb, offsetof(GB_gameboy_t, first_unsaved_data));
    save.cartridge_type = NULL; // Kept from load_rom
    save.rom = NULL; // Kept from load_rom
    save.rom_size = 0; // Kept from load_rom
    save.mbc_ram = NULL;
    save.ram = NULL;
    save.vram = NULL;
    save.screen = NULL; // Kept from user
    save.audio_buffer = NULL; // Kept from user
    save.buffer_size = 0; // Kept from user
    save.sample_rate = 0; // Kept from user
    save.audio_position = 0; // Kept from previous state
    save.vblank_callback = NULL;
    save.user_data = NULL;
    memset(save.keys, 0, sizeof(save.keys)); // Kept from user

    FILE *f = fopen(path, "w");
    if (!f) {
        return errno;
    }

    if (fwrite(&save, 1, offsetof(GB_gameboy_t, first_unsaved_data), f) != offsetof(GB_gameboy_t, first_unsaved_data)) {
        fclose(f);
        return EIO;
    }

    if (fwrite(gb->mbc_ram, 1, gb->mbc_ram_size, f) != gb->mbc_ram_size) {
        fclose(f);
        return EIO;
    }

    if (fwrite(gb->ram, 1, gb->ram_size, f) != gb->ram_size) {
        fclose(f);
        return EIO;
    }

    if (fwrite(gb->vram, 1, gb->vram_size, f) != gb->vram_size) {
        fclose(f);
        return EIO;
    }

    errno = 0;
    fclose(f);
    return errno;
}

int gb_load_state(GB_gameboy_t *gb, const char *path)
{
    GB_gameboy_t save;

    FILE *f = fopen(path, "r");
    if (!f) {
        return errno;
    }

    if (fread(&save, 1, offsetof(GB_gameboy_t, first_unsaved_data), f) != offsetof(GB_gameboy_t, first_unsaved_data)) {
        fclose(f);
        return EIO;
    }

    save.cartridge_type = gb->cartridge_type;
    save.rom = gb->rom;
    save.rom_size = gb->rom_size;
    save.mbc_ram = gb->mbc_ram;
    save.ram = gb->ram;
    save.vram = gb->vram;
    save.screen = gb->screen;
    save.audio_buffer = gb->audio_buffer;
    save.buffer_size = gb->buffer_size;
    save.sample_rate = gb->sample_rate;
    save.audio_position = gb->audio_position;
    save.vblank_callback = gb->vblank_callback;
    save.user_data = gb->user_data;
    memcpy(save.keys, gb->keys, sizeof(save.keys));

    if (gb->magic != save.magic) {
        gb_log(gb, "File is not a save state, or is from an incompatible operating system.\n");
        fclose(f);
        return -1;
    }

    if (gb->version != save.version) {
        gb_log(gb, "Save state is for a different version of SameBoy.\n");
        fclose(f);
        return -1;
    }

    if (gb->mbc_ram_size != save.mbc_ram_size) {
        gb_log(gb, "Save state has non-matching MBC RAM size.\n");
        fclose(f);
        return -1;
    }

    if (gb->ram_size != save.ram_size) {
        gb_log(gb, "Save state has non-matching RAM size. Try changing emulated model.\n");
        fclose(f);
        return -1;
    }

    if (gb->vram_size != save.vram_size) {
        gb_log(gb, "Save state has non-matching VRAM size. Try changing emulated model.\n");
        fclose(f);
        return -1;
    }

    if (fread(gb->mbc_ram, 1, gb->mbc_ram_size, f) != gb->mbc_ram_size) {
        fclose(f);
        return EIO;
    }

    if (fread(gb->ram, 1, gb->ram_size, f) != gb->ram_size) {
        fclose(f);
        return EIO;
    }

    if (fread(gb->vram, 1, gb->vram_size, f) != gb->vram_size) {
        fclose(f);
        return EIO;
    }

    memcpy(gb, &save, offsetof(GB_gameboy_t, first_unsaved_data));
    errno = 0;
    fclose(f);
    return errno;
}

int gb_save_battery(GB_gameboy_t *gb, const char *path)
{
    if (!gb->cartridge_type->has_battery) return 0; // Nothing to save.
    if (gb->mbc_ram_size == 0 && !gb->cartridge_type->has_rtc) return 0; /* Claims to have battery, but has no RAM or RTC */
    FILE *f = fopen(path, "w");
    if (!f) {
        return errno;
    }

    if (fwrite(gb->mbc_ram, 1, gb->mbc_ram_size, f) != gb->mbc_ram_size) {
        fclose(f);
        return EIO;
    }
    if (gb->cartridge_type->has_rtc) {
        if (fwrite(gb->rtc_data, 1, sizeof(gb->rtc_data), f) != sizeof(gb->rtc_data)) {
            fclose(f);
            return EIO;
        }

        if (fwrite(&gb->last_rtc_second, 1, sizeof(gb->last_rtc_second), f) != sizeof(gb->last_rtc_second)) {
            fclose(f);
            return EIO;
        }
    }

    errno = 0;
    fclose(f);
    return errno;
}

/* Loading will silently stop if the format is incomplete */
void gb_load_battery(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        return;
    }

    if (fread(gb->mbc_ram, 1, gb->mbc_ram_size, f) != gb->mbc_ram_size) {
        goto reset_rtc;
    }

    if (fread(gb->rtc_data, 1, sizeof(gb->rtc_data), f) != sizeof(gb->rtc_data)) {
        goto reset_rtc;
    }

    if (fread(&gb->last_rtc_second, 1, sizeof(gb->last_rtc_second), f) != sizeof(gb->last_rtc_second)) {
        goto reset_rtc;
    }

    if (gb->last_rtc_second > time(NULL)) {
        /* We must reset RTC here, or it will not advance. */
        goto reset_rtc;
    }

    if (gb->last_rtc_second < 852076800) { /* 1/1/97. There weren't any RTC games that time,
                                            so if the value we read is lower it means it wasn't
                                            really RTC data. */
        goto reset_rtc;
    }
    goto exit;
reset_rtc:
    gb->last_rtc_second = time(NULL);
    gb->rtc_high |= 0x80; /* This gives the game a hint that the clock should be reset. */
exit:
    fclose(f);
    return;
}

void gb_run(GB_gameboy_t *gb)
{
    update_joyp(gb);
    debugger_run(gb);
    cpu_run(gb);
}

void gb_set_pixels_output(GB_gameboy_t *gb, uint32_t *output)
{
    gb->screen = output;
}

void gb_set_vblank_callback(GB_gameboy_t *gb, GB_vblank_callback_t callback)
{
    gb->vblank_callback = callback;
}

void gb_set_log_callback(GB_gameboy_t *gb, GB_log_callback_t callback)
{
    gb->log_callback = callback;
}

void gb_set_input_callback(GB_gameboy_t *gb, GB_input_callback_t callback)
{
    gb->input_callback = callback;
}

void gb_set_rgb_encode_callback(GB_gameboy_t *gb, GB_rgb_encode_callback_t callback)
{
    gb->rgb_encode_callback = callback;
}

void gb_set_sample_rate(GB_gameboy_t *gb, unsigned int sample_rate)
{
    if (gb->audio_buffer) {
        free(gb->audio_buffer);
    }
    gb->buffer_size = sample_rate / 25; // 40ms delay
    gb->audio_buffer = malloc(gb->buffer_size * 2);
    gb->sample_rate = sample_rate;
    gb->audio_position = 0;
}
