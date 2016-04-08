#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <SDL/SDL.h>

#include "gb.h"

void update_keys_status(GB_gameboy_t *gb)
{
    static bool ctrl = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch( event.type ){
            case SDL_QUIT:
                exit(0);
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
    update_keys_status(gb);

    gb_set_pixels_output(gb, screen->pixels);
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
static uint32_t rgb_encode(GB_gameboy_t *gb, unsigned char r, unsigned char g, unsigned char b)
{
    return SDL_MapRGB(screen->format, r, g, b);
}

GB_gameboy_t gb;

static void debugger_interrupt(int ignore)
{
    gb.debug_stopped = true;
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
        gb_init(&gb);
        if (gb_load_bios(&gb, executable_relative_path("dmg_boot.bin"))) {
            perror("Failed to load boot ROM");
            exit(1);
        }
    }
    else {
        gb_init_cgb(&gb);
        if (gb_load_bios(&gb, executable_relative_path("cgb_boot.bin"))) {
            perror("Failed to load boot ROM");
            exit(1);
        }
    }

    if (gb_load_rom(&gb, argv[argc - 1])) {
        perror("Failed to load ROM");
        exit(1);
    }
    

    signal(SIGINT, debugger_interrupt);

    SDL_Init( SDL_INIT_EVERYTHING );
    screen = SDL_SetVideoMode(160, 144, 32, SDL_SWSURFACE );
#ifdef __APPLE__
    cocoa_disable_filtering();
#endif
    SDL_LockSurface(screen);
    gb_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
    gb.user_data = screen;
    gb_set_pixels_output(&gb, screen->pixels);
    gb_set_rgb_encode_callback(&gb, rgb_encode);

    /* Despite sound not being supported in the SDL port, registers PCM_12 and PCM_34 require
       a sample rate to be set in order to operate. This also means PCM_XX emulation is not
       really accurate yet, as it depends on the sample rate. */
    gb_set_sample_rate(&gb, 96000);

    while (true) {
        gb_run(&gb);
    }
    
    return 0;
}

