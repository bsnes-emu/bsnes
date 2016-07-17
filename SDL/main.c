#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <SDL/SDL.h>

#include "gb.h"

static bool running = false;

void GB_update_keys_status(GB_gameboy_t *gb)
{
    static bool ctrl = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch( event.type ){
            case SDL_QUIT:
                running = false;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                gb->stopped = false;
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        gb->keys[0] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_LEFT:
                        gb->keys[1] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_UP:
                        gb->keys[2] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_DOWN:
                        gb->keys[3] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_x:
                        gb->keys[4] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_z:
                        gb->keys[5] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_BACKSPACE:
                        gb->keys[6] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_RETURN:
                        gb->keys[7] = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_SPACE:
                        gb->turbo = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_LCTRL:
                        ctrl = event.type == SDL_KEYDOWN;
                        break;
                    case SDLK_c:
                        if (ctrl && event.type == SDL_KEYDOWN) {
                            ctrl = false;
                            gb->debug_stopped = true;

                        }
                        break;

                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void vblank(GB_gameboy_t *gb)
{
    SDL_Surface *screen = gb->user_data;
    SDL_Flip(screen);
    GB_update_keys_status(gb);

    GB_set_pixels_output(gb, screen->pixels);
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
#warning Unsupported OS: sameboy will only run from CWD
    /* No OS-specific way, assume running from CWD */
    getcwd(&path[0], sizeof(path) - 1);
    return path;
#endif
#endif
    size_t pos = strlen(path);
    while (pos) {
        pos--;
        if (path[pos] == '/') {
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

static SDL_Surface *screen = NULL;
static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(screen->format, r, g, b);
}

GB_gameboy_t gb;

static void debugger_interrupt(int ignore)
{
    gb.debug_stopped = true;
}


static void audio_callback(void *gb, Uint8 *stream, int len)
{
    GB_apu_copy_buffer(gb, (GB_sample_t *) stream, len / sizeof(GB_sample_t));
}

#ifdef __APPLE__
extern void cocoa_disable_filtering(void);
#endif
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

    if (GB_load_rom(&gb, argv[argc - 1])) {
        perror("Failed to load ROM");
        exit(1);
    }
    

    signal(SIGINT, debugger_interrupt);

    SDL_Init( SDL_INIT_EVERYTHING );
    screen = SDL_SetVideoMode(160, 144, 32, SDL_SWSURFACE );
#ifdef __APPLE__
    cocoa_disable_filtering();
#endif
    /* Configure Screen */
    SDL_LockSurface(screen);
    GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    gb.user_data = screen;
    GB_set_pixels_output(&gb, screen->pixels);
    GB_set_rgb_encode_callback(&gb, rgb_encode);

    /* Configure battery */
    size_t path_length = strlen(argv[argc - 1]);
    char battery_save_path[path_length + 5]; /* At the worst case, size is strlen(path) + 4 bytes for .sav + NULL */
    memcpy(battery_save_path, argv[argc - 1], path_length);

    /* Remove extension */
    for (size_t i = path_length; i--;) {
        if (battery_save_path[i] == '/') break;
        if (battery_save_path[i] == '.') {
            battery_save_path[i] = 0;
            break;
        }
    }

    /* Add .sav */
    strcat(battery_save_path, ".sav");

    GB_load_battery(&gb, battery_save_path);

    /* Configure Audio */
    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq = 96000;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 512;
    want.callback = audio_callback;
    want.userdata = &gb;
    SDL_OpenAudio(&want, &have);
    GB_set_sample_rate(&gb, 96000);
    
    /* Start Audio */
    SDL_PauseAudio(0);

    /* Run emulation */
    running = true;
    while (running) {
        GB_run(&gb);
    }
    SDL_CloseAudio();

    GB_save_battery(&gb, battery_save_path);
    return 0;
}

