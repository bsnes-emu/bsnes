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

enum pending_command {
    GB_SDL_NO_COMMAND,
    GB_SDL_SAVE_STATE_COMMAND,
    GB_SDL_LOAD_STATE_COMMAND,
    GB_SDL_RESET_COMMAND,
    GB_SDL_NEW_FILE_COMMAND,
    GB_SDL_TOGGLE_MODEL_COMMAND,
};

extern enum pending_command pending_command;
extern unsigned command_parameter;

void update_viewport(void);
void cycle_scaling(void);

void run_gui(bool is_running);
#endif
