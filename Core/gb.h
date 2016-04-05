#ifndef gb_h
#define gb_h
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "apu.h"

#define GB_STRUCT_VERSION 6

enum {
    GB_REGISTER_AF,
    GB_REGISTER_BC,
    GB_REGISTER_DE,
    GB_REGISTER_HL,
    GB_REGISTER_SP,
    GB_REGISTERS_16_BIT /* Count */
};

/* Todo: Actually use these! */
enum {
    GB_CARRY_FLAG = 16,
    GB_HALF_CARRY_FLAG = 32,
    GB_SUBSTRACT_FLAG = 64,
    GB_ZERO_FLAG = 128,
};

enum {
    /* Joypad and Serial */
    GB_IO_JOYP       = 0x00, // Joypad (R/W)
    GB_IO_SB         = 0x01, // Serial transfer data (R/W)
    GB_IO_SC         = 0x02, // Serial Transfer Control (R/W)

    /* Missing */

    /* Timers */
    GB_IO_DIV        = 0x04, // Divider Register (R/W)
    GB_IO_TIMA       = 0x05, // Timer counter (R/W)
    GB_IO_TMA        = 0x06, // Timer Modulo (R/W)
    GB_IO_TAC        = 0x07, // Timer Control (R/W)

    /* Missing */

    GB_IO_IF         = 0x0f, // Interrupt Flag (R/W)

    /* Sound */
    GB_IO_NR10       = 0x10, // Channel 1 Sweep register (R/W)
    GB_IO_NR11       = 0x11, // Channel 1 Sound length/Wave pattern duty (R/W)
    GB_IO_NR12       = 0x12, // Channel 1 Volume Envelope (R/W)
    GB_IO_NR13       = 0x13, // Channel 1 Frequency lo (Write Only)
    GB_IO_NR14       = 0x14, // Channel 1 Frequency hi (R/W)
    GB_IO_NR21       = 0x16, // Channel 2 Sound Length/Wave Pattern Duty (R/W)
    GB_IO_NR22       = 0x17, // Channel 2 Volume Envelope (R/W)
    GB_IO_NR23       = 0x18, // Channel 2 Frequency lo data (W)
    GB_IO_NR24       = 0x19, // Channel 2 Frequency hi data (R/W)
    GB_IO_NR30       = 0x1a, // Channel 3 Sound on/off (R/W)
    GB_IO_NR31       = 0x1b, // Channel 3 Sound Length
    GB_IO_NR32       = 0x1c, // Channel 3 Select output level (R/W)
    GB_IO_NR33       = 0x1d, // Channel 3 Frequency's lower data (W)

    GB_IO_NR34       = 0x1e, // Channel 3 Frequency's higher data (R/W)

    /* Missing */

    GB_IO_NR41       = 0x20, // Channel 4 Sound Length (R/W)
    GB_IO_NR42       = 0x21, // Channel 4 Volume Envelope (R/W)
    GB_IO_NR43       = 0x22, // Channel 4 Polynomial Counter (R/W)
    GB_IO_NR44       = 0x23, // Channel 4 Counter/consecutive, Inital (R/W)
    GB_IO_NR50       = 0x24, // Channel control / ON-OFF / Volume (R/W)
    GB_IO_NR51       = 0x25, // Selection of Sound output terminal (R/W)
    GB_IO_NR52       = 0x26, // Sound on/off

    /* Missing */

    GB_IO_WAV_START  = 0x30, // Wave pattern start
    GB_IO_WAV_END    = 0x3f, // Wave pattern end

    /* Graphics */
    GB_IO_LCDC       = 0x40, // LCD Control (R/W)
    GB_IO_STAT       = 0x41, // LCDC Status (R/W)
    GB_IO_SCY        = 0x42, // Scroll Y (R/W)
    GB_IO_SCX        = 0x43, // Scroll X (R/W)
    GB_IO_LY         = 0x44, // LCDC Y-Coordinate (R)
    GB_IO_LYC        = 0x45, // LY Compare (R/W)
    GB_IO_DMA        = 0x46, // DMA Transfer and Start Address (W)
    GB_IO_BGP        = 0x47, // BG Palette Data (R/W) - Non CGB Mode Only
    GB_IO_OBP0       = 0x48, // Object Palette 0 Data (R/W) - Non CGB Mode Only
    GB_IO_OBP1       = 0x49, // Object Palette 1 Data (R/W) - Non CGB Mode Only
    GB_IO_WY         = 0x4a, // Window Y Position (R/W)
    GB_IO_WX         = 0x4b, // Window X Position minus 7 (R/W)
    // Has some undocumented compatibility flags written at boot.
    // Unfortunately it is not readable or writable after boot has finished, so research of this
    // register is quite limited. The value written to this register, however, can be controlled
    // in some cases.
    GB_IO_DMG_EMULATION = 0x4c,

    /* General CGB features */
    GB_IO_KEY1       = 0x4d, // CGB Mode Only - Prepare Speed Switch

    /* Missing */

    GB_IO_VBK        = 0x4f, // CGB Mode Only - VRAM Bank
    GB_IO_BIOS       = 0x50, // Write to disable the BIOS mapping

    /* CGB DMA */
    GB_IO_HDMA1      = 0x51, // CGB Mode Only - New DMA Source, High
    GB_IO_HDMA2      = 0x52, // CGB Mode Only - New DMA Source, Low
    GB_IO_HDMA3      = 0x53, // CGB Mode Only - New DMA Destination, High
    GB_IO_HDMA4      = 0x54, // CGB Mode Only - New DMA Destination, Low
    GB_IO_HDMA5      = 0x55, // CGB Mode Only - New DMA Length/Mode/Start

    /* IR */
    GB_IO_RP         = 0x56, // CGB Mode Only - Infrared Communications Port

    /* Missing */

    /* CGB Paletts */
    GB_IO_BGPI       = 0x68, // CGB Mode Only - Background Palette Index
    GB_IO_BGPD       = 0x69, // CGB Mode Only - Background Palette Data
    GB_IO_OBPI       = 0x6a, // CGB Mode Only - Sprite Palette Index
    GB_IO_OBPD       = 0x6b, // CGB Mode Only - Sprite Palette Data

    // 1 is written for DMG ROMs on a CGB. Does not appear to have an effect.
    GB_IO_DMG_EMULATION_INDICATION   = 0x6c, // (FEh) Bit 0 (Read/Write)

    /* Missing */

    GB_IO_SVBK       = 0x70, // CGB Mode Only - WRAM Bank
    GB_IO_UNKNOWN2   = 0x72, // (00h) - Bit 0-7 (Read/Write)
    GB_IO_UNKNOWN3   = 0x73, // (00h) - Bit 0-7 (Read/Write)
    GB_IO_UNKNOWN4   = 0x74, // (00h) - Bit 0-7 (Read/Write) - CGB Mode Only
    GB_IO_UNKNOWN5   = 0x75, // (8Fh) - Bit 4-6 (Read/Write)
    GB_IO_PCM_12     = 0x76, // Channels 1 and 2 amplitudes
    GB_IO_PCM_34     = 0x77, // Channels 3 and 4 amplitudes
    GB_IO_UNKNOWN8   = 0x7F, // Unknown, write only
};

#define LCDC_PERIOD 70224
#define CPU_FREQUENCY 0x400000
#define DIV_CYCLES (0x100)
#define FRAME_LENGTH 16742706 // in nanoseconds

typedef enum {
    GB_LOG_BOLD = 1,
    GB_LOG_DASHED_UNDERLINE = 2,
    GB_LOG_UNDERLINE = 4,
    GB_LOG_UNDERLINE_MASK =  GB_LOG_DASHED_UNDERLINE | GB_LOG_UNDERLINE
} gb_log_attributes;

struct GB_gameboy_s;
typedef struct GB_gameboy_s GB_gameboy_t;
typedef void (*GB_vblank_callback_t)(GB_gameboy_t *gb);
typedef void (*GB_log_callback_t)(GB_gameboy_t *gb, const char *string, gb_log_attributes attributes);
typedef char *(*GB_input_callback_t)(GB_gameboy_t *gb);
typedef uint32_t (*GB_rgb_encode_callback_t)(GB_gameboy_t *gb, unsigned char r, unsigned char g, unsigned char b);

typedef struct {
    enum {
        NO_MBC,
        MBC1,
        MBC2,
        MBC3,
        MBC4, // Does this exist???
        MBC5,
    } mbc_type;
    bool has_ram;
    bool has_battery;
    bool has_rtc;
    bool has_rumble;
} GB_cartridge_t;

typedef struct GB_gameboy_s{
    uintptr_t magic; // States are currently platform dependent
    int version; // and version dependent
    /* Registers */
    unsigned short pc;
    unsigned short registers[GB_REGISTERS_16_BIT];
    bool ime;
    unsigned char interrupt_enable;

    /* CPU and General Hardware Flags*/
    bool cgb_mode;
    bool is_cgb;
    bool cgb_double_speed;
    bool halted;
    bool stopped;

    /* HDMA */
    bool hdma_on;
    bool hdma_on_hblank;
    unsigned char hdma_steps_left;
    unsigned short hdma_cycles;
    unsigned short hdma_current_src, hdma_current_dest;

    /* Memory */
    unsigned char *rom;
    size_t rom_size;
    unsigned short mbc_rom_bank;

    const GB_cartridge_t *cartridge_type;
    unsigned char *mbc_ram;
    unsigned char mbc_ram_bank;
    size_t mbc_ram_size;
    bool mbc_ram_enable;
    bool mbc_ram_banking;

    unsigned char *ram;
    unsigned long ram_size; // Different between CGB and DMG
    unsigned char cgb_ram_bank;

    unsigned char hram[0xFFFF - 0xFF80];
    unsigned char io_registers[0x80];

    /* Video Display */
    unsigned char *vram;
    unsigned long vram_size; // Different between CGB and DMG
    unsigned char cgb_vram_bank;
    unsigned char oam[0xA0];
    unsigned char background_palletes_data[0x40];
    unsigned char sprite_palletes_data[0x40];
    uint32_t background_palletes_rgb[0x20];
    uint32_t sprite_palletes_rgb[0x20];
    bool ly144_bug_oam;
    bool ly144_bug_hblank;
    signed short previous_lcdc_x;
    signed short line_x_bias;
    bool effective_window_enabled;
    unsigned char effective_window_y;

    unsigned char bios[0x900];
    bool bios_finished;

    /* Timing */
    signed long last_vblank;
    unsigned long display_cycles;
    unsigned long div_cycles;
    unsigned long tima_cycles;
    unsigned long dma_cycles;
    double apu_cycles;

    /* APU */
    GB_apu_t apu;
    int16_t *audio_buffer;
    unsigned int buffer_size;
    unsigned int sample_rate;
    unsigned int audio_position;
    volatile bool audio_copy_in_progress;
    bool audio_stream_started; // detects first copy request to minimize lag
    
    /* I/O */
    uint32_t *screen;
    GB_vblank_callback_t vblank_callback;

    bool keys[8];

    /* RTC */
    union {
        struct {
            unsigned char rtc_seconds;
            unsigned char rtc_minutes;
            unsigned char rtc_hours;
            unsigned char rtc_days;
            unsigned char rtc_high;
        };
        unsigned char rtc_data[5];
    };
    time_t last_rtc_second;

    /* Unsaved User */
    struct {} first_unsaved_data;
    bool turbo;
    bool debug_stopped;
    unsigned short breakpoint;
    GB_log_callback_t log_callback;
    GB_input_callback_t input_callback;
    GB_rgb_encode_callback_t rgb_encode_callback;
    void *user_data;
    int debug_call_depth;
    bool debug_fin_command, debug_next_command;

} GB_gameboy_t;

#ifndef __printflike
/* Missing from Linux headers. */
#define __printflike(fmtarg, firstvararg) \
__attribute__((__format__ (__printf__, fmtarg, firstvararg)))
#endif

void gb_init(GB_gameboy_t *gb);
void gb_init_cgb(GB_gameboy_t *gb);
void gb_free(GB_gameboy_t *gb);
int gb_load_bios(GB_gameboy_t *gb, const char *path);
int gb_load_rom(GB_gameboy_t *gb, const char *path);
int gb_save_battery(GB_gameboy_t *gb, const char *path);
void gb_load_battery(GB_gameboy_t *gb, const char *path);
int gb_save_state(GB_gameboy_t *gb, const char *path);
int gb_load_state(GB_gameboy_t *gb, const char *path);
void gb_run(GB_gameboy_t *gb);
void gb_set_pixels_output(GB_gameboy_t *gb, uint32_t *output);
void gb_set_vblank_callback(GB_gameboy_t *gb, GB_vblank_callback_t callback);
void gb_set_log_callback(GB_gameboy_t *gb, GB_log_callback_t callback);
void gb_log(GB_gameboy_t *gb, const char *fmt, ...) __printflike(2, 3);
void gb_attributed_log(GB_gameboy_t *gb, gb_log_attributes attributes, const char *fmt, ...) __printflike(3, 4);
void gb_set_input_callback(GB_gameboy_t *gb, GB_input_callback_t callback);
void gb_set_sample_rate(GB_gameboy_t *gb, unsigned int sample_rate);
void gb_set_rgb_encode_callback(GB_gameboy_t *gb, GB_rgb_encode_callback_t callback);

#endif /* gb_h */
