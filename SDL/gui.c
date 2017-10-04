#include <SDL2/SDL.h>
#include <stdbool.h>
#include "utils.h"
#include "gui.h"
#include "font.h"

static const SDL_Color gui_palette[4] = {{8, 24, 16,}, {57, 97, 57,}, {132, 165, 99}, {198, 222, 140}};
static uint32_t gui_palette_native[4];

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_PixelFormat *pixel_format = NULL;
enum scaling_mode scaling_mode = GB_SDL_SCALING_INTEGER_FACTOR;


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

void cycle_scaling(void)
{
    scaling_mode++;
    scaling_mode %= GB_SDL_SCALING_MAX;
    update_viewport();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void update_viewport(void)
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

void show_help(void)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", help, window);
}

/* Does NOT check for bounds! */
static void draw_char(uint32_t *buffer, unsigned char ch, uint32_t color)
{
    if (ch < ' ' || ch > '~') {
        ch = '?';
    }
    
    uint8_t *data = &font[(ch - ' ') * GLYPH_WIDTH * GLYPH_HEIGHT];
    
    for (unsigned y = GLYPH_HEIGHT; y--;) {
        for (unsigned x = GLYPH_WIDTH; x--;) {
            if (*(data++)) {
                (*buffer) = color;
            }
            buffer++;
        }
        buffer += 160 - GLYPH_WIDTH;
    }
}

/* Does NOT check for bounds! */
static void draw_bordered_char(uint32_t *buffer, unsigned char ch, uint32_t color, uint32_t border)
{
    draw_char(buffer - 1, ch, border);
    draw_char(buffer + 1, ch, border);
    draw_char(buffer - 160, ch, border);
    draw_char(buffer + 160, ch, border);
    draw_char(buffer, ch, color);
}

static void draw_text(uint32_t *buffer, unsigned x, unsigned y, const char *string, uint32_t color, uint32_t border)
{
    unsigned orig_x = x;
    while (*string) {
        if (*string == '\n') {
            x = orig_x;
            y += GLYPH_HEIGHT + 4;
            string++;
            continue;
        }
        
        if (x == 0 || x > 160 - GLYPH_WIDTH - 1 || y == 0 || y > 144 - GLYPH_HEIGHT - 1) {
            break;
        }
        
        draw_bordered_char(&buffer[x + 160 * y], *string, color, border);
        x += GLYPH_WIDTH;
        string++;
    }
}

static void draw_text_centered(uint32_t *buffer, unsigned y, const char *string, uint32_t color, uint32_t border)
{
    draw_text(buffer, 160 / 2 - (unsigned) strlen(string) * GLYPH_WIDTH / 2, y, string, color, border);
}


extern void set_filename(const char *new_filename, bool new_should_free);
void run_gui(void)
{
    /* Draw the "Drop file" screen */
    static SDL_Surface *converted_background = NULL;
    if (!converted_background) {
        SDL_Surface *background = SDL_LoadBMP(executable_relative_path("background.bmp"));
        SDL_SetPaletteColors(background->format->palette, gui_palette, 0, 4);
        converted_background = SDL_ConvertSurface(background, pixel_format, 0);
        SDL_LockSurface(converted_background);
        SDL_FreeSurface(background);
        
        for (unsigned i = 4; i--; ) {
            gui_palette_native[i] = SDL_MapRGB(pixel_format, gui_palette[i].r, gui_palette[i].g, gui_palette[i].b);
        }
    }

    uint32_t pixels[160 * 144];
    memcpy(pixels, converted_background->pixels, sizeof(pixels));
    
    draw_text_centered(pixels, 116, "Drop a GB or GBC", gui_palette_native[3], gui_palette_native[0]);
    draw_text_centered(pixels, 128, "file to play", gui_palette_native[3], gui_palette_native[0]);
    
    SDL_UpdateTexture(texture, NULL, pixels, 160 * sizeof (uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
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
                set_filename(event.drop.file, true);
                return;
            }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_TAB) {
                    cycle_scaling();
                }
#ifndef __APPLE__
                else if (event.key.keysym.sym == SDLK_F1) {
                    show_help();
                }
#else
                else if (event.key.keysym.sym == SDLK_QUESTION || (event.key.keysym.sym && (event.key.keysym.mod & KMOD_SHIFT))) {
                    show_help();
                }
#endif
                break;
        }
    }
}
