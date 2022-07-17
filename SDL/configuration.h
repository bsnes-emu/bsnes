#ifndef configuration_h
#define configuration_h

#include <SDL.h>
#include <Core/gb.h>
#include "shader.h"

enum scaling_mode {
    GB_SDL_SCALING_ENTIRE_WINDOW,
    GB_SDL_SCALING_KEEP_RATIO,
    GB_SDL_SCALING_INTEGER_FACTOR,
    GB_SDL_SCALING_MAX,
};

typedef enum {
    JOYPAD_BUTTON_RIGHT,
    JOYPAD_BUTTON_LEFT,
    JOYPAD_BUTTON_UP,
    JOYPAD_BUTTON_DOWN,
    JOYPAD_BUTTON_A,
    JOYPAD_BUTTON_B,
    JOYPAD_BUTTON_SELECT,
    JOYPAD_BUTTON_START,
    JOYPAD_BUTTON_MENU,
    JOYPAD_BUTTON_TURBO,
    JOYPAD_BUTTON_REWIND,
    JOYPAD_BUTTON_SLOW_MOTION,
    JOYPAD_BUTTONS_MAX
} joypad_button_t;

typedef enum {
    JOYPAD_AXISES_X,
    JOYPAD_AXISES_Y,
    JOYPAD_AXISES_MAX
} joypad_axis_t;

typedef struct {
    SDL_Scancode keys[9];
    GB_color_correction_mode_t color_correction_mode;
    enum scaling_mode scaling_mode;
    uint8_t blending_mode;
    
    GB_highpass_mode_t highpass_mode;
    
    bool _deprecated_div_joystick;
    bool _deprecated_flip_joystick_bit_1;
    bool _deprecated_swap_joysticks_bits_1_and_2;
    
    char filter[32];
    enum {
        MODEL_DMG,
        MODEL_CGB,
        MODEL_AGB,
        MODEL_SGB,
        MODEL_MGB,
        MODEL_MAX,
    } model;
    
    /* v0.11 */
    uint32_t rewind_length;
    SDL_Scancode keys_2[32]; /* Rewind and underclock, + padding for the future */
    uint8_t joypad_configuration[32]; /* 12 Keys + padding for the future*/;
    uint8_t joypad_axises[JOYPAD_AXISES_MAX];
    
    /* v0.12 */
    enum {
        SGB_NTSC,
        SGB_PAL,
        SGB_2,
        SGB_MAX
    } sgb_revision;
    
    /* v0.13 */
    uint8_t dmg_palette;
    GB_border_mode_t border_mode;
    uint8_t volume;
    GB_rumble_mode_t rumble_mode;
    
    uint8_t default_scale;
    
    /* v0.14 */
    unsigned padding;
    uint8_t color_temperature;
    char bootrom_path[4096];
    uint8_t interference_volume;
    GB_rtc_mode_t rtc_mode;
    
    /* v0.14.4 */
    bool osd;
    
    struct __attribute__((packed, aligned(4))) {
        
        /* v0.15 */
        bool allow_mouse_controls;
        uint8_t cgb_revision;
        /* v0.15.1 */
        char audio_driver[16];
        /* v0.15.2 */
        bool allow_background_controllers;
        bool gui_pallete_enabled; // Change the GUI palette only once the user changed the DMG palette
        char dmg_palette_name[25];
    };
} configuration_t;

extern configuration_t configuration;

#endif
