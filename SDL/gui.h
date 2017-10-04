#ifndef gui_h
#define gui_h

#include <SDL2/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_PixelFormat *pixel_format;

enum scaling_mode {
    GB_SDL_SCALING_ENTIRE_WINDOW,
    GB_SDL_SCALING_KEEP_RATIO,
    GB_SDL_SCALING_INTEGER_FACTOR,
    GB_SDL_SCALING_MAX,
};

extern enum scaling_mode scaling_mode;

void update_viewport(void);
void cycle_scaling(void);
void show_help(void);

void run_gui(void);
#endif
