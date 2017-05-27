#include <stdbool.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include "gb.h"

#include "utils.h"

#ifndef _WIN32
#define AUDIO_FREQUENCY 96000
#else
/* Windows (well, at least my VM) can't handle 96KHz sound well :( */
#define AUDIO_FREQUENCY 44100
#endif

#ifdef __APPLE__
#define MODIFIER_NAME "Cmd"
#else
#define MODIFIER_NAME "Ctrl"
#endif

static const char help[] =
"Drop a GB or GBC ROM file to play.\n"
"\n"
"Controls:\n"
"    D-Pad: Arrow Keys\n"
"    A: X\n"
"    B: Z\n"
"    Start: Enter\n"
"    Select: Backspace\n"
"\n"
"Keyboard Shortcuts: \n"
"    Restart: " MODIFIER_NAME "+R\n"
"    Pause: " MODIFIER_NAME "+P\n"
"    Turbo: Space\n"
#ifdef __APPLE__
"    Mute/Unmute: " MODIFIER_NAME "+Shift+M\n"
#else
"    Mute/Unmute: " MODIFIER_NAME "+M\n"
#endif
"    Save state: " MODIFIER_NAME "+Number (0-9)\n"
"    Load state: " MODIFIER_NAME "+Shift+Number (0-9)\n"
"    Cycle between DMG/CGB emulation: " MODIFIER_NAME "+T\n"
"    Cycle scaling modes: Tab"
;

GB_gameboy_t gb;
static bool dmg = false;
static bool paused = false;
static uint32_t pixels[160*144];

static char *filename = NULL;
static bool should_free_filename = false;
static char *battery_save_path_ptr;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_PixelFormat *pixel_format = NULL;
static SDL_AudioSpec want_aspec, have_aspec;

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

static const char *end_capturing_logs(bool show_popup, bool should_exit)
{
    GB_set_log_callback(&gb, NULL);
    if (captured_log[0] == 0) {
        free(captured_log);
        captured_log = NULL;
    }
    else {
        if (show_popup) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", captured_log, window);
        }
        if (should_exit) {
            exit(1);
        }
    }
    return captured_log;
}

static enum {
    GB_SDL_NO_COMMAND,
    GB_SDL_SAVE_STATE_COMMAND,
    GB_SDL_LOAD_STATE_COMMAND,
    GB_SDL_RESET_COMMAND,
    GB_SDL_NEW_FILE_COMMAND,
    GB_SDL_TOGGLE_MODEL_COMMAND,
} pending_command;

static enum {
    GB_SDL_SCALING_ENTIRE_WINDOW,
    GB_SDL_SCALING_KEEP_RATIO,
    GB_SDL_SCALING_INTEGER_FACTOR,
    GB_SDL_SCALING_MAX,
} scaling_mode = GB_SDL_SCALING_INTEGER_FACTOR;
static unsigned command_parameter;

static void update_viewport(void)
{
    int win_width, win_height;
    SDL_GetWindowSize(window, &win_width, &win_height);
    double x_factor = win_width / 160.0;
    double y_factor = win_height / 144.0;
    
    if (scaling_mode == GB_SDL_SCALING_INTEGER_FACTOR) {
        x_factor = (int)(x_factor);
        y_factor = (int)(y_factor);
    }

    if (scaling_mode != GB_SDL_SCALING_ENTIRE_WINDOW) {
        if (x_factor > y_factor) {
            x_factor = y_factor;
        }
        else {
            y_factor = x_factor;
        }
    }
    
    unsigned new_width = x_factor * 160;
    unsigned new_height = y_factor * 144;
    
    SDL_Rect rect = (SDL_Rect){(win_width  - new_width) / 2, (win_height - new_height) /2,
        new_width, new_height};
    SDL_RenderSetViewport(renderer, &rect);
}

static void cycle_scaling(void)
{
    scaling_mode++;
    scaling_mode %= GB_SDL_SCALING_MAX;
    update_viewport();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

static void handle_events(GB_gameboy_t *gb)
{
#ifdef __APPLE__
#define MODIFIER KMOD_GUI
#else
#define MODIFIER KMOD_CTRL
#endif
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            case SDL_QUIT:
                GB_save_battery(gb, battery_save_path_ptr);
                exit(0);
                
            case SDL_DROPFILE: {
                if (should_free_filename) {
                    SDL_free(filename);
                }
                filename = event.drop.file;
                should_free_filename = true;
                pending_command = GB_SDL_NEW_FILE_COMMAND;
                break;
            }
                
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    update_viewport();
                }
            }
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_c:
                        if (event.type == SDL_KEYDOWN && (event.key.keysym.mod & KMOD_CTRL)) {
                            GB_debugger_break(gb);
                            
                        }
                        break;
                        
                    case SDLK_r:
                        if (event.key.keysym.mod & MODIFIER) {
                            pending_command = GB_SDL_RESET_COMMAND;
                        }
                        break;
                        
                    case SDLK_t:
                        if (event.key.keysym.mod & MODIFIER) {
                            pending_command = GB_SDL_TOGGLE_MODEL_COMMAND;
                        }
                        break;
                    
                    case SDLK_p:
                        if (event.key.keysym.mod & MODIFIER) {
                            paused = !paused;
                        }
                        break;
                        
                    case SDLK_m:
                        if (event.key.keysym.mod & MODIFIER) {
#ifdef __APPLE__
                            // Can't override CMD+M (Minimize) in SDL
                            if (!(event.key.keysym.mod & KMOD_SHIFT)) {
                                break;
                            }
#endif
                            SDL_PauseAudio(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING? true : false);
                        }
                        break;
                        
                    case SDLK_TAB:
                        cycle_scaling();
                        break;
#ifndef __APPLE__
                    case SDLK_F1:
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", help, window);
                        break;
#else
                    case SDLK_SLASH:
                        if (!(event.key.keysym.sym && (event.key.keysym.mod & KMOD_SHIFT))) {
                            break;
                        }
                    case SDLK_QUESTION:
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", help, window);
#endif
                        
                    default:
                        /* Save states */
                        if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
                            if (event.key.keysym.mod & MODIFIER) {
                                command_parameter = event.key.keysym.sym - SDLK_0;
                                
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
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        GB_set_key_state(gb, GB_KEY_RIGHT, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_LEFT:
                        GB_set_key_state(gb, GB_KEY_LEFT, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_UP:
                        GB_set_key_state(gb, GB_KEY_UP, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_DOWN:
                        GB_set_key_state(gb, GB_KEY_DOWN, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_x:
                        GB_set_key_state(gb, GB_KEY_A, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_z:
                        GB_set_key_state(gb, GB_KEY_B, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_BACKSPACE:
                        GB_set_key_state(gb, GB_KEY_SELECT, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_RETURN:
                        GB_set_key_state(gb, GB_KEY_START, event.type == SDL_KEYDOWN);
                        break;
                    case SDLK_SPACE:
                        GB_set_turbo_mode(gb, event.type == SDL_KEYDOWN, false);
                        break;
                }
                break;
            default:
                break;
        }
    }
}

static void vblank(GB_gameboy_t *gb)
{
    SDL_UpdateTexture(texture, NULL, pixels, 160 * sizeof (uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    handle_events(gb);
}


static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(pixel_format, r, g, b);
}

static void debugger_interrupt(int ignore)
{
    /* ^C twice to exit */
    if (GB_debugger_is_stopped(&gb)) {
        exit(0);
    }
    GB_debugger_break(&gb);
}


static void audio_callback(void *gb, Uint8 *stream, int len)
{
    if (GB_is_inited(gb)) {
        GB_apu_copy_buffer(gb, (GB_sample_t *) stream, len / sizeof(GB_sample_t));
    }
    else {
        memset(stream, 0, len);
    }
}
    
static bool handle_pending_command(void)
{
    switch (pending_command) {
        case GB_SDL_LOAD_STATE_COMMAND:
        case GB_SDL_SAVE_STATE_COMMAND: {
            char save_path[strlen(filename) + 4];
            char save_extension[] = ".s0";
            save_extension[2] += command_parameter;
            replace_extension(filename, strlen(filename), save_path, save_extension);
            
            start_capturing_logs();
            if (pending_command == GB_SDL_LOAD_STATE_COMMAND) {
                GB_load_state(&gb, save_path);
            }
            else {
                GB_save_state(&gb, save_path);
            }
            end_capturing_logs(true, false);
            return false;
        }
            
        case GB_SDL_RESET_COMMAND:
            GB_reset(&gb);
            return false;
            
        case GB_SDL_NO_COMMAND:
            return false;
            
        case GB_SDL_NEW_FILE_COMMAND:
            return true;
            
        case GB_SDL_TOGGLE_MODEL_COMMAND:
            dmg = !dmg;
            return true;
    }
    return false;
}

static void run(void)
{
restart:
    if (GB_is_inited(&gb)) {
        GB_switch_model_and_reset(&gb, !dmg);
    }
    else {
        if (dmg) {
            GB_init(&gb);
        }
        else {
            GB_init_cgb(&gb);
        }
        
        GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
        GB_set_pixels_output(&gb, pixels);
        GB_set_rgb_encode_callback(&gb, rgb_encode);
        GB_set_sample_rate(&gb, have_aspec.freq);
    }
    
    start_capturing_logs();
    if (dmg) {
        GB_load_boot_rom(&gb, executable_relative_path("dmg_boot.bin"));
    }
    else {
        GB_load_boot_rom(&gb, executable_relative_path("cgb_boot.bin"));
    }
    end_capturing_logs(true, true);
    
    start_capturing_logs();
    GB_load_rom(&gb, filename);
    end_capturing_logs(true, true);
    
    size_t path_length = strlen(filename);
    
    /* Configure battery */
    char battery_save_path[path_length + 5]; /* At the worst case, size is strlen(path) + 4 bytes for .sav + NULL */
    replace_extension(filename, path_length, battery_save_path, ".sav");
    battery_save_path_ptr = battery_save_path;
    GB_load_battery(&gb, battery_save_path);
    
    /* Configure symbols */
    GB_debugger_load_symbol_file(&gb, executable_relative_path("registers.sym"));
    
    char symbols_path[path_length + 5];
    replace_extension(filename, path_length, symbols_path, ".sym");
    GB_debugger_load_symbol_file(&gb, symbols_path);
    
    /* Run emulation */
    while (true) {
        if (paused) {
            SDL_WaitEvent(NULL);
            handle_events(&gb);
        }
        else {
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
    
int main(int argc, char **argv)
{
#define str(x) #x
#define xstr(x) str(x)
    fprintf(stderr, "SameBoy v" xstr(VERSION) "\n");

    if (argc > 3) {
usage:
        fprintf(stderr, "Usage: %s [--dmg] [rom]\n", argv[0]);
        exit(1);
    }
    
    for (unsigned i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--dmg") == 0) {
            if (dmg) {
                goto usage;
            }
            dmg = true;
        }
        else if (!filename) {
            filename = argv[i];
        }
        else {
            goto usage;
        }
    }

    signal(SIGINT, debugger_interrupt);

    SDL_Init( SDL_INIT_EVERYTHING );
    
    window = SDL_CreateWindow("SameBoy v" xstr(VERSION), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              160 * 2, 144 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowMinimumSize(window, 160, 144);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, 160, 144);
    
    pixel_format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));

    /* Configure Audio */
    memset(&want_aspec, 0, sizeof(want_aspec));
    want_aspec.freq = AUDIO_FREQUENCY;
    want_aspec.format = AUDIO_S16SYS;
    want_aspec.channels = 2;
#if SDL_COMPILEDVERSION == 2005 && defined(__APPLE__)
    /* SDL 2.0.5 on macOS introduced a bug where certain combinations of buffer lengths and frequencies 
       fail to produce audio correctly. This bug was fixed 2.0.6. */
    want_aspec.samples = 2048;
#else
    want_aspec.samples = 512;
#endif
    want_aspec.callback = audio_callback;
    want_aspec.userdata = &gb;
    SDL_OpenAudio(&want_aspec, &have_aspec);
    
    /* Start Audio */
    SDL_PauseAudio(false);

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    
    if (filename == NULL) {
        /* Draw the "Drop file" screen */
        SDL_Surface *drop_backround = SDL_LoadBMP(executable_relative_path("drop.bmp"));
        SDL_Surface *drop_converted = SDL_ConvertSurface(drop_backround, pixel_format, 0);
        SDL_LockSurface(drop_converted);
        SDL_UpdateTexture(texture, NULL, drop_converted->pixels, drop_converted->pitch);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(drop_converted);
        SDL_FreeSurface(drop_backround);
        SDL_Event event;
        while (SDL_WaitEvent(&event))
        {
            switch (event.type) {
                case SDL_QUIT: {
                    exit(0);
                }
                case SDL_WINDOWEVENT: {
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        update_viewport();
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, texture, NULL, NULL);
                        SDL_RenderPresent(renderer);
                    }
                    break;
                }
                case SDL_DROPFILE: {
                    filename = event.drop.file;
                    should_free_filename = true;
                    goto start;
                }
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_TAB) {
                        cycle_scaling();
                    }
#ifndef __APPLE__
                    else if (event.key.keysym.sym == SDLK_F1) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", help, window);
                    }
#else
                    else if (event.key.keysym.sym == SDLK_QUESTION || (event.key.keysym.sym && (event.key.keysym.mod & KMOD_SHIFT))) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", help, window);
                    }
#endif
                    break;
            }
        }
    }
start:
    run(); // Never returns
    return 0;
}
