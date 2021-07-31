#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <OpenDialog/open_dialog.h>
#include <SDL.h>
#include <Core/gb.h>
#include "utils.h"
#include "gui.h"
#include "shader.h"
#include "audio/audio.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

static bool stop_on_start = false;
GB_gameboy_t gb;
static bool paused = false;
static uint32_t pixel_buffer_1[256 * 224], pixel_buffer_2[256 * 224];
static uint32_t *active_pixel_buffer = pixel_buffer_1, *previous_pixel_buffer = pixel_buffer_2;
static bool underclock_down = false, rewind_down = false, do_rewind = false, rewind_paused = false, turbo_down = false;
static double clock_mutliplier = 1.0;

static char *filename = NULL;
static typeof(free) *free_function = NULL;
static char *battery_save_path_ptr = NULL;
static SDL_GLContext gl_context = NULL;

bool uses_gl(void)
{
    return gl_context;
}

void set_filename(const char *new_filename, typeof(free) *new_free_function)
{
    if (filename && free_function) {
        free_function(filename);
    }
    filename = (char *) new_filename;
    free_function = new_free_function;
}

static char *captured_log = NULL;

static void log_capture_callback(GB_gameboy_t *gb, const char *string, GB_log_attributes attributes)
{
    size_t current_len = strlen(captured_log);
    size_t len_to_add = strlen(string);
    captured_log = realloc(captured_log, current_len + len_to_add + 1);
    memcpy(captured_log + current_len, string, len_to_add);
    captured_log[current_len + len_to_add] = 0;
}

static void start_capturing_logs(void)
{
    if (captured_log != NULL) {
        free(captured_log);
    }
    captured_log = malloc(1);
    captured_log[0] = 0;
    GB_set_log_callback(&gb, log_capture_callback);
}

static const char *end_capturing_logs(bool show_popup, bool should_exit, uint32_t popup_flags, const char *title)
{
    GB_set_log_callback(&gb, NULL);
    if (captured_log[0] == 0) {
        free(captured_log);
        captured_log = NULL;
    }
    else {
        if (show_popup) {
            SDL_ShowSimpleMessageBox(popup_flags, title, captured_log, window);
        }
        if (should_exit) {
            exit(1);
        }
    }
    return captured_log;
}

static void update_palette(void)
{
    switch (configuration.dmg_palette) {
        case 1:
            GB_set_palette(&gb, &GB_PALETTE_DMG);
            break;
            
        case 2:
            GB_set_palette(&gb, &GB_PALETTE_MGB);
            break;
            
        case 3:
            GB_set_palette(&gb, &GB_PALETTE_GBL);
            break;
            
        default:
            GB_set_palette(&gb, &GB_PALETTE_GREY);
    }
}

static void screen_size_changed(void)
{
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING,
                                GB_get_screen_width(&gb), GB_get_screen_height(&gb));
    
    SDL_SetWindowMinimumSize(window, GB_get_screen_width(&gb), GB_get_screen_height(&gb));
    
    update_viewport();
}

static void open_menu(void)
{
    bool audio_playing = GB_audio_is_playing();
    if (audio_playing) {
        GB_audio_set_paused(true);
    }
    size_t previous_width = GB_get_screen_width(&gb);
    run_gui(true);
    SDL_ShowCursor(SDL_DISABLE);
    if (audio_playing) {
        GB_audio_set_paused(false);
    }
    GB_set_color_correction_mode(&gb, configuration.color_correction_mode);
    GB_set_light_temperature(&gb, (configuration.color_temperature - 10.0) / 10.0);
    GB_set_interference_volume(&gb, configuration.interference_volume / 100.0);
    GB_set_border_mode(&gb, configuration.border_mode);
    update_palette();
    GB_set_highpass_filter_mode(&gb, configuration.highpass_mode);
    GB_set_rewind_length(&gb, configuration.rewind_length);
    GB_set_rtc_mode(&gb, configuration.rtc_mode);
    if (previous_width != GB_get_screen_width(&gb)) {
        screen_size_changed();
    }
}

static void handle_events(GB_gameboy_t *gb)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                pending_command = GB_SDL_QUIT_COMMAND;
                break;
                
            case SDL_DROPFILE: {
                if (GB_is_stave_state(event.drop.file)) {
                    dropped_state_file = event.drop.file;
                    pending_command = GB_SDL_LOAD_STATE_FROM_FILE_COMMAND;
                }
                else {
                    set_filename(event.drop.file, SDL_free);
                    pending_command = GB_SDL_NEW_FILE_COMMAND;
                }
                break;
            }
                
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    update_viewport();
                }
                break;
            }
                
            case SDL_JOYBUTTONUP:
            case SDL_JOYBUTTONDOWN: {
                joypad_button_t button = get_joypad_button(event.jbutton.button);
                if ((GB_key_t) button < GB_KEY_MAX) {
                    GB_set_key_state(gb, (GB_key_t) button, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (button == JOYPAD_BUTTON_TURBO) {
                    GB_audio_clear_queue();
                    turbo_down = event.type == SDL_JOYBUTTONDOWN;
                    GB_set_turbo_mode(gb, turbo_down, turbo_down && rewind_down);
                }
                else if (button == JOYPAD_BUTTON_SLOW_MOTION) {
                    underclock_down = event.type == SDL_JOYBUTTONDOWN;
                }
                else if (button == JOYPAD_BUTTON_REWIND) {
                    rewind_down = event.type == SDL_JOYBUTTONDOWN;
                    if (event.type == SDL_JOYBUTTONUP) {
                        rewind_paused = false;
                    }
                    GB_set_turbo_mode(gb, turbo_down, turbo_down && rewind_down);
                }
                else if (button == JOYPAD_BUTTON_MENU && event.type == SDL_JOYBUTTONDOWN) {
                    open_menu();
                }
            }
                break;
                
            case SDL_JOYAXISMOTION: {
                static bool axis_active[2] = {false, false};
                joypad_axis_t axis = get_joypad_axis(event.jaxis.axis);
                if (axis == JOYPAD_AXISES_X) {
                    if (event.jaxis.value > JOYSTICK_HIGH) {
                        axis_active[0] = true;
                        GB_set_key_state(gb, GB_KEY_RIGHT, true);
                        GB_set_key_state(gb, GB_KEY_LEFT, false);
                    }
                    else if (event.jaxis.value < -JOYSTICK_HIGH) {
                        axis_active[0] = true;
                        GB_set_key_state(gb, GB_KEY_RIGHT, false);
                        GB_set_key_state(gb, GB_KEY_LEFT, true);
                    }
                    else if (axis_active[0] && event.jaxis.value < JOYSTICK_LOW && event.jaxis.value > -JOYSTICK_LOW) {
                        axis_active[0] = false;
                        GB_set_key_state(gb, GB_KEY_RIGHT, false);
                        GB_set_key_state(gb, GB_KEY_LEFT, false);
                    }
                }
                else if (axis == JOYPAD_AXISES_Y) {
                    if (event.jaxis.value > JOYSTICK_HIGH) {
                        axis_active[1] = true;
                        GB_set_key_state(gb, GB_KEY_DOWN, true);
                        GB_set_key_state(gb, GB_KEY_UP, false);
                    }
                    else if (event.jaxis.value < -JOYSTICK_HIGH) {
                        axis_active[1] = true;
                        GB_set_key_state(gb, GB_KEY_DOWN, false);
                        GB_set_key_state(gb, GB_KEY_UP, true);
                    }
                    else if (axis_active[1] && event.jaxis.value < JOYSTICK_LOW && event.jaxis.value > -JOYSTICK_LOW) {
                        axis_active[1] = false;
                        GB_set_key_state(gb, GB_KEY_DOWN, false);
                        GB_set_key_state(gb, GB_KEY_UP, false);
                    }
                }
            }
                break;
                
            case SDL_JOYHATMOTION: {
                uint8_t value = event.jhat.value;
                int8_t updown =
                value == SDL_HAT_LEFTUP || value == SDL_HAT_UP || value == SDL_HAT_RIGHTUP ? -1 : (value == SDL_HAT_LEFTDOWN || value == SDL_HAT_DOWN || value == SDL_HAT_RIGHTDOWN ? 1 : 0);
                int8_t leftright =
                value == SDL_HAT_LEFTUP || value == SDL_HAT_LEFT || value == SDL_HAT_LEFTDOWN ? -1 : (value == SDL_HAT_RIGHTUP || value == SDL_HAT_RIGHT || value == SDL_HAT_RIGHTDOWN ? 1 : 0);
                
                GB_set_key_state(gb, GB_KEY_LEFT, leftright == -1);
                GB_set_key_state(gb, GB_KEY_RIGHT, leftright == 1);
                GB_set_key_state(gb, GB_KEY_UP, updown == -1);
                GB_set_key_state(gb, GB_KEY_DOWN, updown == 1);
                break;
            };
                
            case SDL_KEYDOWN:
                switch (event_hotkey_code(&event)) {
                    case SDL_SCANCODE_ESCAPE: {
                        open_menu();
                        break;
                    }
                    case SDL_SCANCODE_C:
                        if (event.type == SDL_KEYDOWN && (event.key.keysym.mod & KMOD_CTRL)) {
                            GB_debugger_break(gb);
                        }
                        break;
                        
                    case SDL_SCANCODE_R:
                        if (event.key.keysym.mod & MODIFIER) {
                            pending_command = GB_SDL_RESET_COMMAND;
                        }
                        break;
                        
                    case SDL_SCANCODE_O: {
                        if (event.key.keysym.mod & MODIFIER) {
                            char *filename = do_open_rom_dialog();
                            if (filename) {
                                set_filename(filename, free);
                                pending_command = GB_SDL_NEW_FILE_COMMAND;
                            }
                        }
                        break;
                    }
                        
                    case SDL_SCANCODE_P:
                        if (event.key.keysym.mod & MODIFIER) {
                            paused = !paused;
                        }
                        break;
                    case SDL_SCANCODE_M:
                        if (event.key.keysym.mod & MODIFIER) {
#ifdef __APPLE__
                            // Can't override CMD+M (Minimize) in SDL
                            if (!(event.key.keysym.mod & KMOD_SHIFT)) {
                                break;
                            }
#endif
                            GB_audio_set_paused(GB_audio_is_playing());
                        }
                        break;
                        
                    case SDL_SCANCODE_F:
                        if (event.key.keysym.mod & MODIFIER) {
                            if ((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) == false) {
                                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                            }
                            else {
                                SDL_SetWindowFullscreen(window, 0);
                            }
                            update_viewport();
                        }
                        break;
                        
                    default:
                        /* Save states */
                        if (event.key.keysym.scancode >= SDL_SCANCODE_1 && event.key.keysym.scancode <= SDL_SCANCODE_0) {
                            if (event.key.keysym.mod & MODIFIER) {
                                command_parameter = (event.key.keysym.scancode - SDL_SCANCODE_1 + 1) % 10;
                                
                                if (event.key.keysym.mod & KMOD_SHIFT) {
                                    pending_command = GB_SDL_LOAD_STATE_COMMAND;
                                }
                                else {
                                    pending_command = GB_SDL_SAVE_STATE_COMMAND;
                                }
                            }
                        }
                        break;
                }
            case SDL_KEYUP: // Fallthrough
                if (event.key.keysym.scancode == configuration.keys[8]) {
                    turbo_down = event.type == SDL_KEYDOWN;
                    GB_audio_clear_queue();
                    GB_set_turbo_mode(gb, turbo_down, turbo_down && rewind_down);
                }
                else if (event.key.keysym.scancode == configuration.keys_2[0]) {
                    rewind_down = event.type == SDL_KEYDOWN;
                    if (event.type == SDL_KEYUP) {
                        rewind_paused = false;
                    }
                    GB_set_turbo_mode(gb, turbo_down, turbo_down && rewind_down);
                }
                else if (event.key.keysym.scancode == configuration.keys_2[1]) {
                    underclock_down = event.type == SDL_KEYDOWN;
                }
                else {
                    for (unsigned i = 0; i < GB_KEY_MAX; i++) {
                        if (event.key.keysym.scancode == configuration.keys[i]) {
                            GB_set_key_state(gb, i, event.type == SDL_KEYDOWN);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(pixel_format, r, g, b);
}

static void vblank(GB_gameboy_t *gb)
{
    if (underclock_down && clock_mutliplier > 0.5) {
        clock_mutliplier -= 1.0/16;
        GB_set_clock_multiplier(gb, clock_mutliplier);
    }
    else if (!underclock_down && clock_mutliplier < 1.0) {
        clock_mutliplier += 1.0/16;
        GB_set_clock_multiplier(gb, clock_mutliplier);
    }
    
    if (turbo_down) {
        show_osd_text("Fast forward...");
    }
    else if (underclock_down) {
        show_osd_text("Slow motion...");
    }
    else if (rewind_down) {
        show_osd_text("Rewinding...");
    }
    
    if (osd_countdown && configuration.osd) {
        unsigned width = GB_get_screen_width(gb);
        unsigned height = GB_get_screen_height(gb);
        draw_text(active_pixel_buffer,
                  width, height, 8, height - 8 - osd_text_lines * 12, osd_text,
                  rgb_encode(gb, 255, 255, 255), rgb_encode(gb, 0, 0, 0),
                  true);
        osd_countdown--;
    }
    if (configuration.blending_mode) {
        render_texture(active_pixel_buffer, previous_pixel_buffer);
        uint32_t *temp = active_pixel_buffer;
        active_pixel_buffer = previous_pixel_buffer;
        previous_pixel_buffer = temp;
        GB_set_pixels_output(gb, active_pixel_buffer);
    }
    else {
        render_texture(active_pixel_buffer, NULL);
    }
    do_rewind = rewind_down;
    handle_events(gb);
}

static void rumble(GB_gameboy_t *gb, double amp)
{
    SDL_HapticRumblePlay(haptic, amp, 250);
}

static void debugger_interrupt(int ignore)
{
    if (!GB_is_inited(&gb)) return;
    /* ^C twice to exit */
    if (GB_debugger_is_stopped(&gb)) {
        GB_save_battery(&gb, battery_save_path_ptr);
        exit(0);
    }
    GB_debugger_break(&gb);
}

static void gb_audio_callback(GB_gameboy_t *gb, GB_sample_t *sample)
{    
    if (turbo_down) {
        static unsigned skip = 0;
        skip++;
        if (skip == GB_audio_get_frequency() / 8) {
            skip = 0;
        }
        if (skip > GB_audio_get_frequency() / 16) {
            return;
        }
    }
    
    if (GB_audio_get_queue_length() / sizeof(*sample) > GB_audio_get_frequency() / 4) {
        return;
    }
    
    if (configuration.volume != 100) {
        sample->left = sample->left * configuration.volume / 100;
        sample->right = sample->right * configuration.volume / 100;
    }
    
    GB_audio_queue_sample(sample);
    
}

    
static bool handle_pending_command(void)
{
    switch (pending_command) {
        case GB_SDL_LOAD_STATE_COMMAND:
        case GB_SDL_SAVE_STATE_COMMAND: {
            char save_path[strlen(filename) + 5];
            char save_extension[] = ".s0";
            save_extension[2] += command_parameter;
            replace_extension(filename, strlen(filename), save_path, save_extension);
            
            start_capturing_logs();
            bool success;
            if (pending_command == GB_SDL_LOAD_STATE_COMMAND) {
                int result = GB_load_state(&gb, save_path);
                if (result == ENOENT) {
                    char save_extension[] = ".sn0";
                    save_extension[3] += command_parameter;
                    replace_extension(filename, strlen(filename), save_path, save_extension);
                    start_capturing_logs();
                    result = GB_load_state(&gb, save_path);
                }
                success = result == 0;
            }
            else {
                success = GB_save_state(&gb, save_path) == 0;
            }
            end_capturing_logs(true,
                               false,
                               success? SDL_MESSAGEBOX_INFORMATION : SDL_MESSAGEBOX_ERROR,
                               success? "Notice" : "Error");
            if (success) {
                show_osd_text(pending_command == GB_SDL_LOAD_STATE_COMMAND? "State loaded" : "State saved");
            }
            return false;
        }
    
        case GB_SDL_LOAD_STATE_FROM_FILE_COMMAND:
            start_capturing_logs();
            bool success = GB_load_state(&gb, dropped_state_file) == 0;
            end_capturing_logs(true,
                               false,
                               success? SDL_MESSAGEBOX_INFORMATION : SDL_MESSAGEBOX_ERROR,
                               success? "Notice" : "Error");
            SDL_free(dropped_state_file);
            if (success) {
                show_osd_text("State loaded");
            }
            return false;
            
        case GB_SDL_NO_COMMAND:
            return false;
            
        case GB_SDL_RESET_COMMAND:
        case GB_SDL_NEW_FILE_COMMAND:
            GB_save_battery(&gb, battery_save_path_ptr);
            return true;
            
        case GB_SDL_QUIT_COMMAND:
            GB_save_battery(&gb, battery_save_path_ptr);
            exit(0);
    }
    return false;
}

static void load_boot_rom(GB_gameboy_t *gb, GB_boot_rom_t type)
{
    static const char *const names[] = {
        [GB_BOOT_ROM_DMG0] = "dmg0_boot.bin",
        [GB_BOOT_ROM_DMG] = "dmg_boot.bin",
        [GB_BOOT_ROM_MGB] = "mgb_boot.bin",
        [GB_BOOT_ROM_SGB] = "sgb_boot.bin",
        [GB_BOOT_ROM_SGB2] = "sgb2_boot.bin",
        [GB_BOOT_ROM_CGB0] = "cgb0_boot.bin",
        [GB_BOOT_ROM_CGB] = "cgb_boot.bin",
        [GB_BOOT_ROM_AGB] = "agb_boot.bin",
    };
    bool use_built_in = true;
    if (configuration.bootrom_path[0]) {
        static char path[4096];
        snprintf(path, sizeof(path), "%s/%s", configuration.bootrom_path, names[type]);
        use_built_in = GB_load_boot_rom(gb, path);
    }
    if (use_built_in) {
        start_capturing_logs();
        GB_load_boot_rom(gb, resource_path(names[type]));
        end_capturing_logs(true, false, SDL_MESSAGEBOX_ERROR, "Error");
    }
}

static void run(void)
{
    SDL_ShowCursor(SDL_DISABLE);
    GB_model_t model;
    pending_command = GB_SDL_NO_COMMAND;
restart:
    model = (GB_model_t [])
    {
        [MODEL_DMG] = GB_MODEL_DMG_B,
        [MODEL_CGB] = GB_MODEL_CGB_E,
        [MODEL_AGB] = GB_MODEL_AGB,
        [MODEL_SGB] = (GB_model_t [])
        {
            [SGB_NTSC] = GB_MODEL_SGB_NTSC,
            [SGB_PAL] = GB_MODEL_SGB_PAL,
            [SGB_2] = GB_MODEL_SGB2,
        }[configuration.sgb_revision],
    }[configuration.model];
    
    if (GB_is_inited(&gb)) {
        GB_switch_model_and_reset(&gb, model);
    }
    else {
        GB_init(&gb, model);
        
        GB_set_boot_rom_load_callback(&gb, load_boot_rom);
        GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
        GB_set_pixels_output(&gb, active_pixel_buffer);
        GB_set_rgb_encode_callback(&gb, rgb_encode);
        GB_set_rumble_callback(&gb, rumble);
        GB_set_rumble_mode(&gb, configuration.rumble_mode);
        GB_set_sample_rate(&gb, GB_audio_get_frequency());
        GB_set_color_correction_mode(&gb, configuration.color_correction_mode);
        GB_set_light_temperature(&gb, (configuration.color_temperature - 10.0) / 10.0);
        GB_set_interference_volume(&gb, configuration.interference_volume / 100.0);
        update_palette();
        if ((unsigned)configuration.border_mode <= GB_BORDER_ALWAYS) {
            GB_set_border_mode(&gb, configuration.border_mode);
        }
        GB_set_highpass_filter_mode(&gb, configuration.highpass_mode);
        GB_set_rewind_length(&gb, configuration.rewind_length);
        GB_set_rtc_mode(&gb, configuration.rtc_mode);
        GB_set_update_input_hint_callback(&gb, handle_events);
        GB_apu_set_sample_callback(&gb, gb_audio_callback);
    }
    if (stop_on_start) {
        stop_on_start = false;
        GB_debugger_break(&gb);
    }

    bool error = false;
    GB_debugger_clear_symbols(&gb);
    start_capturing_logs();
    size_t path_length = strlen(filename);
    char extension[4] = {0,};
    if (path_length > 4) {
        if (filename[path_length - 4] == '.') {
            extension[0] = tolower((unsigned char)filename[path_length - 3]);
            extension[1] = tolower((unsigned char)filename[path_length - 2]);
            extension[2] = tolower((unsigned char)filename[path_length - 1]);
        }
    }
    if (strcmp(extension, "isx") == 0) {
        error = GB_load_isx(&gb, filename);
        /* Configure battery */
        char battery_save_path[path_length + 5]; /* At the worst case, size is strlen(path) + 4 bytes for .sav + NULL */
        replace_extension(filename, path_length, battery_save_path, ".ram");
        battery_save_path_ptr = battery_save_path;
        GB_load_battery(&gb, battery_save_path);
    }
    else {
        GB_load_rom(&gb, filename);
    }
    end_capturing_logs(true, error, SDL_MESSAGEBOX_WARNING, "Warning");
    
    static char start_text[64];
    static char title[17];
    GB_get_rom_title(&gb, title);
    sprintf(start_text, "SameBoy v" GB_VERSION "\n%s\n%08X", title, GB_get_rom_crc32(&gb));
    show_osd_text(start_text);

    
    /* Configure battery */
    char battery_save_path[path_length + 5]; /* At the worst case, size is strlen(path) + 4 bytes for .sav + NULL */
    replace_extension(filename, path_length, battery_save_path, ".sav");
    battery_save_path_ptr = battery_save_path;
    GB_load_battery(&gb, battery_save_path);
    
    /* Configure symbols */
    GB_debugger_load_symbol_file(&gb, resource_path("registers.sym"));
    
    char symbols_path[path_length + 5];
    replace_extension(filename, path_length, symbols_path, ".sym");
    GB_debugger_load_symbol_file(&gb, symbols_path);
        
    screen_size_changed();

    /* Run emulation */
    while (true) {
        if (paused || rewind_paused) {
            SDL_WaitEvent(NULL);
            handle_events(&gb);
        }
        else {
            if (do_rewind) {
                GB_rewind_pop(&gb);
                if (turbo_down) {
                    GB_rewind_pop(&gb);
                }
                if (!GB_rewind_pop(&gb)) {
                    rewind_paused = true;
                }
                do_rewind = false;
            }
            GB_run(&gb);
        }
        
        /* These commands can't run in the handle_event function, because they're not safe in a vblank context. */
        if (handle_pending_command()) {
            pending_command = GB_SDL_NO_COMMAND;
            goto restart;
        }
        pending_command = GB_SDL_NO_COMMAND;
    }
}

static char prefs_path[1024] = {0, };

static void save_configuration(void)
{
    FILE *prefs_file = fopen(prefs_path, "wb");
    if (prefs_file) {
        fwrite(&configuration, 1, sizeof(configuration), prefs_file);
        fclose(prefs_file);
    }
}

static bool get_arg_flag(const char *flag, int *argc, char **argv)
{
    for (unsigned i = 1; i < *argc; i++) {
        if (strcmp(argv[i], flag) == 0) {
            (*argc)--;
            argv[i] = argv[*argc];
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    SetProcessDPIAware();
#endif
    fprintf(stderr, "SameBoy v" GB_VERSION "\n");
    
    bool fullscreen = get_arg_flag("--fullscreen", &argc, argv) || get_arg_flag("-f", &argc, argv);
    bool nogl = get_arg_flag("--nogl", &argc, argv);
    stop_on_start = get_arg_flag("--stop-debugger", &argc, argv) || get_arg_flag("-s", &argc, argv);

    if (argc > 2 || (argc == 2 && argv[1][0] == '-')) {
        fprintf(stderr, "Usage: %s [--fullscreen|-f] [--nogl] [--stop-debugger|-s] [rom]\n", argv[0]);
        exit(1);
    }
    
    if (argc == 2) {
        filename = argv[1];
    }

    signal(SIGINT, debugger_interrupt);

    SDL_Init(SDL_INIT_EVERYTHING);
    
    strcpy(prefs_path, resource_path("prefs.bin"));
    if (access(prefs_path, R_OK | W_OK) != 0) {
        char *prefs_dir = SDL_GetPrefPath("", "SameBoy");
        snprintf(prefs_path, sizeof(prefs_path) - 1, "%sprefs.bin", prefs_dir);
        SDL_free(prefs_dir);
    }
    
    FILE *prefs_file = fopen(prefs_path, "rb");
    if (prefs_file) {
        fread(&configuration, 1, sizeof(configuration), prefs_file);
        fclose(prefs_file);
        
        /* Sanitize for stability */
        configuration.color_correction_mode %= GB_COLOR_CORRECTION_LOW_CONTRAST +1;
        configuration.scaling_mode %= GB_SDL_SCALING_MAX;
        configuration.default_scale %= GB_SDL_DEFAULT_SCALE_MAX + 1;
        configuration.blending_mode %= GB_FRAME_BLENDING_MODE_ACCURATE + 1;
        configuration.highpass_mode %= GB_HIGHPASS_MAX;
        configuration.model %= MODEL_MAX;
        configuration.sgb_revision %= SGB_MAX;
        configuration.dmg_palette %= 3;
        configuration.border_mode %= GB_BORDER_ALWAYS + 1;
        configuration.rumble_mode %= GB_RUMBLE_ALL_GAMES + 1;
        configuration.color_temperature %= 21;
        configuration.bootrom_path[sizeof(configuration.bootrom_path) - 1] = 0;
    }
    
    if (configuration.model >= MODEL_MAX) {
        configuration.model = MODEL_CGB;
    }

    if (configuration.default_scale == 0) {
        configuration.default_scale = 2;
    }
    
    atexit(save_configuration);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("SameBoy v" GB_VERSION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              160 * configuration.default_scale, 144 * configuration.default_scale, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        fputs(SDL_GetError(), stderr);
        exit(1);
    }
    SDL_SetWindowMinimumSize(window, 160, 144);
    
    if (fullscreen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    
    gl_context = nogl? NULL : SDL_GL_CreateContext(window);
    
    GLint major = 0, minor = 0;
    if (gl_context) {
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
    }
    
    if (gl_context && major * 0x100 + minor < 0x302) {
        SDL_GL_DeleteContext(gl_context);
        gl_context = NULL;
    }
    
    if (gl_context == NULL) {
        renderer = SDL_CreateRenderer(window, -1, 0);
        texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, 160, 144);
        pixel_format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));
    }
    else {
        pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_ABGR8888);
    }
    
    GB_audio_init();

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    
    if (!init_shader_with_name(&shader, configuration.filter)) {
        init_shader_with_name(&shader, "NearestNeighbor");
    }
    update_viewport();
    
    if (filename == NULL) {
        stop_on_start = false;
        run_gui(false);
    }
    else {
        connect_joypad();
    }
    GB_audio_set_paused(false);
    run(); // Never returns
    return 0;
}
