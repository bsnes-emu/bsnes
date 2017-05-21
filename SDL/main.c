#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <SDL2/SDL.h>
#ifndef _WIN32
#define AUDIO_FREQUENCY 96000
#else
/* Windows (well, at least my VM) can't handle 96KHz sound well :( */
#define AUDIO_FREQUENCY 44100
#include <direct.h>
#include <windows.h>
#define snprintf _snprintf
#endif

#if defined(__APPLE__) && SDL_MAJOR_VERSION == 2 && SDL_MINOR_VERSION == 0 && SDL_PATCHLEVEL == 5
#error You are using SDL2-2.0.5, which contains a bug that prevents 96KHz audio playback on macOS. Use SDL2-2.0.4 or alternatively modify this file to reduce the frequency to 44100 and disable this error.
#endif

#include "gb.h"

static char *filename;
static char *battery_save_path_ptr;
static void replace_extension(const char *src, size_t length, char *dest, const char *ext);

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_PixelFormat *pixel_format = NULL;
static uint32_t pixels[160*144];
GB_gameboy_t gb;

static enum {
    GB_SDL_NO_COMMAND,
    GB_SDL_SAVE_STATE_COMMAND,
    GB_SDL_LOAD_STATE_COMMAND,
    GB_SDL_RESET_COMMAND,
} pending_command;
static unsigned command_parameter;

static void handle_events(GB_gameboy_t *gb)
{
    static bool ctrl = false;
    static bool shift = false;
#ifdef __APPLE__
    static bool cmd = false;
#define MODIFIER cmd
#else
#define MODIFIER ctrl
#endif
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch( event.type ){
            case SDL_QUIT:
                GB_save_battery(gb, battery_save_path_ptr);
                exit(0);
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_c:
                        if (ctrl && event.type == SDL_KEYDOWN) {
                            ctrl = false;
                            GB_debugger_break(gb);
                            
                        }
                        break;
                        
                    case SDLK_r:
                        if (MODIFIER) {
                            pending_command = GB_SDL_RESET_COMMAND;
                        }
                        break;
                        
                    case SDLK_m:
                        if (MODIFIER) {
#ifdef __APPLE__
                            // Can't over CMD+M (Minimize) in SDL
                            if (!shift) {
                                break;
                            }
#endif
                            SDL_PauseAudio(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING? true : false);
                        }
                        break;
                        
                    default:
                        /* Save states */
                        if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
                            if (MODIFIER) {
                                command_parameter = event.key.keysym.sym - SDLK_0;
                                
                                if (shift) {
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
                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                        ctrl = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift = event.type == SDL_KEYDOWN;
                        break;
#ifdef __APPLE__
                    case SDLK_LGUI:
                    case SDLK_RGUI:
                        cmd = event.type == SDL_KEYDOWN;
                        break;
#endif
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

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

static const char *executable_folder(void)
{
    static char path[1024] = {0,};
    if (path[0]) {
        return path;
    }
    /* Ugly unportable code! :( */
#ifdef __APPLE__
    unsigned int length = sizeof(path) - 1;
    _NSGetExecutablePath(&path[0], &length);
#else
#ifdef __linux__
    ssize_t length = readlink("/proc/self/exe", &path[0], sizeof(path) - 1);
    assert (length != -1);
#else
#ifdef _WIN32
    HMODULE hModule = GetModuleHandle(NULL);
    GetModuleFileName(hModule, path, sizeof(path) - 1);
#else
    /* No OS-specific way, assume running from CWD */
    getcwd(&path[0], sizeof(path) - 1);
    return path;
#endif
#endif
#endif
    size_t pos = strlen(path);
    while (pos) {
        pos--;
#ifdef _WIN32
        if (path[pos] == '\\') {
#else
        if (path[pos] == '/') {
#endif
            path[pos] = 0;
            break;
        }
    }
    return path;
}

static char *executable_relative_path(const char *filename)
{
    static char path[1024];
    snprintf(path, sizeof(path), "%s/%s", executable_folder(), filename);
    return path;
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
    GB_apu_copy_buffer(gb, (GB_sample_t *) stream, len / sizeof(GB_sample_t));
}

static void replace_extension(const char *src, size_t length, char *dest, const char *ext)
{
    memcpy(dest, src, length);
    dest[length] = 0;

    /* Remove extension */
    for (size_t i = length; i--;) {
        if (dest[i] == '/') break;
        if (dest[i] == '.') {
            dest[i] = 0;
            break;
        }
    }

    /* Add new extension */
    strcat(dest, ext);
}

int main(int argc, char **argv)
{
    bool dmg = false;

#define str(x) #x
#define xstr(x) str(x)
    fprintf(stderr, "SameBoy v" xstr(VERSION) "\n");

    if (argc == 1 || argc > 3) {
usage:
        fprintf(stderr, "Usage: %s [--dmg] rom\n", argv[0]);
        exit(1);
    }

    if (argc == 3) {
        if (strcmp(argv[1], "--dmg") == 0) {
            dmg = true;
        }
        else {
            goto usage;
        }
    }


    if (dmg) {
        GB_init(&gb);
        if (GB_load_boot_rom(&gb, executable_relative_path("dmg_boot.bin"))) {
            perror("Failed to load boot ROM");
            exit(1);
        }
    }
    else {
        GB_init_cgb(&gb);
        if (GB_load_boot_rom(&gb, executable_relative_path("cgb_boot.bin"))) {
            perror("Failed to load boot ROM");
            exit(1);
        }
    }

    filename = argv[argc - 1];

    if (GB_load_rom(&gb, filename)) {
        perror("Failed to load ROM");
        exit(1);
    }
    

    signal(SIGINT, debugger_interrupt);

    SDL_Init( SDL_INIT_EVERYTHING );
    
    window = SDL_CreateWindow("SameBoy v" xstr(VERSION), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              160, 144, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);
    
    texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, 160, 144);
    
    pixel_format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));

    /* Configure Screen */
    GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    GB_set_pixels_output(&gb, pixels);
    GB_set_rgb_encode_callback(&gb, rgb_encode);

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

    /* Configure Audio */
    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = AUDIO_FREQUENCY;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 512;
    want.callback = audio_callback;
    want.userdata = &gb;
    SDL_OpenAudio(&want, &have);
    GB_set_sample_rate(&gb, have.freq);
    
    /* Start Audio */
    SDL_PauseAudio(false);

    /* Run emulation */
    while (true) {
        GB_run(&gb);
        switch (pending_command) {
            case GB_SDL_LOAD_STATE_COMMAND:
            case GB_SDL_SAVE_STATE_COMMAND: {
                char save_path[strlen(filename) + 4];
                char save_extension[] = ".s0";
                save_extension[2] += command_parameter;
                replace_extension(filename, strlen(filename), save_path, save_extension);
                
                if (pending_command == GB_SDL_LOAD_STATE_COMMAND) {
                    GB_load_state(&gb, save_path);
                }
                else {
                    GB_save_state(&gb, save_path);
                }
            break;
            }
                
            case GB_SDL_RESET_COMMAND:
                GB_reset(&gb);
                break;
                
            case GB_SDL_NO_COMMAND:
                break;
        }
        pending_command = GB_SDL_NO_COMMAND;
    }
}

