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
enum pending_command pending_command;
unsigned command_parameter;

#ifdef __APPLE__
#define MODIFIER_NAME " " CMD_STRING
#else
#define MODIFIER_NAME CTRL_STRING
#endif

configuration_t configuration =
{
    {   SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_X,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_BACKSPACE,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_SPACE
    },
    GB_COLOR_CORRECTION_EMULATE_HARDWARE,
    GB_SDL_SCALING_INTEGER_FACTOR,
};


static const char *help[] ={
"Drop a GB or GBC ROM\n"
"file to play.\n"
"\n"
"Keyboard Shortcuts:\n"
" Open Menu:        Escape\n"
" Reset:             " MODIFIER_NAME "+R\n"
" Pause:             " MODIFIER_NAME "+P\n"
" Toggle DMG/CGB:    " MODIFIER_NAME "+T\n"
" Save state:    " MODIFIER_NAME "+(0-9)\n"
" Load state:  " MODIFIER_NAME "+" SHIFT_STRING "+(0-9)\n"
#ifdef __APPLE__
" Mute/Unmute:     " MODIFIER_NAME "+" SHIFT_STRING "+M\n"
#else
" Mute/Unmute:       " MODIFIER_NAME "+M\n"
#endif
" Break Debugger:    " CTRL_STRING "+C"
};

void update_viewport(void)
{
    int win_width, win_height;
    SDL_GetWindowSize(window, &win_width, &win_height);
    double x_factor = win_width / 160.0;
    double y_factor = win_height / 144.0;
    
    if (configuration.scaling_mode == GB_SDL_SCALING_INTEGER_FACTOR) {
        x_factor = (int)(x_factor);
        y_factor = (int)(y_factor);
    }
    
    if (configuration.scaling_mode != GB_SDL_SCALING_ENTIRE_WINDOW) {
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

/* Does NOT check for bounds! */
static void draw_char(uint32_t *buffer, unsigned char ch, uint32_t color)
{
    if (ch < ' ' || ch > font_max) {
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

static void draw_unbordered_text(uint32_t *buffer, unsigned x, unsigned y, const char *string, uint32_t color)
{
    unsigned orig_x = x;
    while (*string) {
        if (*string == '\n') {
            x = orig_x;
            y += GLYPH_HEIGHT + 4;
            string++;
            continue;
        }
        
        if (x > 160 - GLYPH_WIDTH || y == 0 || y > 144 - GLYPH_HEIGHT) {
            break;
        }
        
        draw_char(&buffer[x + 160 * y], *string, color);
        x += GLYPH_WIDTH;
        string++;
    }
}

static void draw_text(uint32_t *buffer, unsigned x, unsigned y, const char *string, uint32_t color, uint32_t border)
{
    draw_unbordered_text(buffer, x - 1, y, string, border);
    draw_unbordered_text(buffer, x + 1, y, string, border);
    draw_unbordered_text(buffer, x, y - 1, string, border);
    draw_unbordered_text(buffer, x, y + 1, string, border);
    draw_unbordered_text(buffer, x, y, string, color);
}

enum decoration {
    DECORATION_NONE,
    DECORATION_SELECTION,
    DECORATION_ARROWS,
};

static void draw_text_centered(uint32_t *buffer, unsigned y, const char *string, uint32_t color, uint32_t border, enum decoration decoration)
{
    unsigned x = 160 / 2 - (unsigned) strlen(string) * GLYPH_WIDTH / 2;
    draw_text(buffer, x, y, string, color, border);
    switch (decoration) {
        case DECORATION_SELECTION:
            draw_text(buffer, x - GLYPH_WIDTH, y, SELECTION_STRING, color, border);
            break;
        case DECORATION_ARROWS:
            draw_text(buffer, x - GLYPH_WIDTH, y, LEFT_ARROW_STRING, color, border);
            draw_text(buffer, 160 - x, y, RIGHT_ARROW_STRING, color, border);
            break;
            
        case DECORATION_NONE:
            break;
    }
}

struct menu_item {
    const char *string;
    void (*handler)(unsigned);
    const char *(*value_getter)(unsigned);
    void (*backwards_handler)(unsigned);
};
static const struct menu_item *current_menu = NULL;
static const struct menu_item *root_menu = NULL;
static unsigned current_selection = 0;

static enum {
    SHOWING_DROP_MESSAGE,
    SHOWING_MENU,
    SHOWING_HELP,
    WAITING_FOR_KEY,
} gui_state;

static void item_exit(unsigned index)
{
    pending_command = GB_SDL_QUIT_COMMAND;
}

static unsigned current_help_page = 0;
static void item_help(unsigned index)
{
    current_help_page = 0;
    gui_state = SHOWING_HELP;
}

static void enter_graphics_menu(unsigned index);
static void enter_controls_menu(unsigned index);

static const struct menu_item paused_menu[] = {
    {"Resume", NULL},
    {"Graphic Options", enter_graphics_menu},
    {"Controls", enter_controls_menu},
    {"Help", item_help},
    {"Exit", item_exit},
    {NULL,}
};

static const struct menu_item nonpaused_menu[] = {
    {"Graphic Options", enter_graphics_menu},
    {"Controls", enter_controls_menu},
    {"Help", item_help},
    {"Exit", item_exit},
    {NULL,}
};

const char *current_scaling_mode(unsigned index)
{
    return (const char *[]){"Fill Entire Window", "Retain Aspect Ratio", "Retain Integer Factor"}
        [configuration.scaling_mode];
}

const char *current_color_correction_mode(unsigned index)
{
    return (const char *[]){"Disabled", "Correct Color Curves", "Emulate Hardware", "Preserve Brightness"}
        [configuration.color_correction_mode];
}

void cycle_scaling(unsigned index)
{
    configuration.scaling_mode++;
    if (configuration.scaling_mode == GB_SDL_SCALING_MAX) {
        configuration.scaling_mode = 0;
    }
    update_viewport();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void cycle_scaling_backwards(unsigned index)
{
    if (configuration.scaling_mode == 0) {
        configuration.scaling_mode = GB_SDL_SCALING_MAX - 1;
    }
    else {
        configuration.scaling_mode--;
    }
    update_viewport();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

static void cycle_color_correction(unsigned index)
{
    if (configuration.color_correction_mode == GB_COLOR_CORRECTION_PRESERVE_BRIGHTNESS) {
        configuration.color_correction_mode = GB_COLOR_CORRECTION_DISABLED;
    }
    else {
        configuration.color_correction_mode++;
    }
}

static void cycle_color_correction_backwards(unsigned index)
{
    if (configuration.color_correction_mode == GB_COLOR_CORRECTION_DISABLED) {
        configuration.color_correction_mode = GB_COLOR_CORRECTION_PRESERVE_BRIGHTNESS;
    }
    else {
        configuration.color_correction_mode--;
    }
}


static void return_to_root_menu(unsigned index)
{
    current_menu = root_menu;
    current_selection = 0;
}

static const struct menu_item graphics_menu[] = {
    {"Scaling Mode:", cycle_scaling, current_scaling_mode, cycle_scaling_backwards},
    {"Color Correction:", cycle_color_correction, current_color_correction_mode, cycle_color_correction_backwards},
    {"Back", return_to_root_menu},
    {NULL,}
};

static void enter_graphics_menu(unsigned index)
{
    current_menu = graphics_menu;
    current_selection = 0;
}

static const char *key_name(unsigned index)
{
    return SDL_GetScancodeName(configuration.keys[index]);
}

static void modify_key(unsigned index)
{
    gui_state = WAITING_FOR_KEY;
}

static const struct menu_item controls_menu[] = {
    {"Right:", modify_key, key_name,},
    {"Left:", modify_key, key_name,},
    {"Up:", modify_key, key_name,},
    {"Down:", modify_key, key_name,},
    {"A:", modify_key, key_name,},
    {"B:", modify_key, key_name,},
    {"Select:", modify_key, key_name,},
    {"Start:", modify_key, key_name,},
    {"Turbo:", modify_key, key_name,},
    {"Back", return_to_root_menu},
    {NULL,}
};

static void enter_controls_menu(unsigned index)
{
    current_menu = controls_menu;
    current_selection = 0;
}

extern void set_filename(const char *new_filename, bool new_should_free);
void run_gui(bool is_running)
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
    SDL_Event event = {0,};
    gui_state = is_running? SHOWING_MENU : SHOWING_DROP_MESSAGE;
    bool should_render = true;
    current_menu = root_menu = is_running? paused_menu : nonpaused_menu;
    current_selection = 0;
    do {
        switch (event.type) {
            case SDL_QUIT: {
                if (!is_running) {
                    exit(0);
                }
                else {
                    pending_command = GB_SDL_QUIT_COMMAND;
                    return;
                }
                
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
                pending_command = GB_SDL_NEW_FILE_COMMAND;
                return;
            }
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    if (is_running) {
                        return;
                    }
                    else {
                        if (gui_state == SHOWING_DROP_MESSAGE) {
                            gui_state = SHOWING_MENU;
                        }
                        else if (gui_state == SHOWING_MENU) {
                            gui_state = SHOWING_DROP_MESSAGE;
                        }
                        current_selection = 0;
                        current_menu = root_menu;
                        should_render = true;
                    }
                }
                
                if (gui_state == SHOWING_MENU) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && current_menu[current_selection + 1].string) {
                        current_selection++;
                        should_render = true;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_UP && current_selection) {
                        current_selection--;
                        should_render = true;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                        if (current_menu[current_selection].handler) {
                            current_menu[current_selection].handler(current_selection);
                            if (pending_command) {
                                if (!is_running && pending_command == GB_SDL_QUIT_COMMAND) {
                                    exit(0);
                                }
                                return;
                            }
                            should_render = true;
                        }
                        else {
                            return;
                        }
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT && current_menu[current_selection].backwards_handler) {
                        current_menu[current_selection].handler(current_selection);
                        should_render = true;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT && current_menu[current_selection].backwards_handler) {
                        current_menu[current_selection].backwards_handler(current_selection);
                        should_render = true;
                    }
                }
                else if (gui_state == SHOWING_HELP) {
                    current_help_page++;
                    if (current_help_page == sizeof(help) / sizeof(help[0])) {
                        gui_state = SHOWING_MENU;
                    }
                    should_render = true;
                }
                else if (gui_state == WAITING_FOR_KEY) {
                    configuration.keys[current_selection] = event.key.keysym.scancode;
                    gui_state = SHOWING_MENU;
                    should_render = true;
                }
                break;
        }
        
        if (should_render) {
            should_render = false;
            memcpy(pixels, converted_background->pixels, sizeof(pixels));
            
            switch (gui_state) {
                case SHOWING_DROP_MESSAGE:
                    draw_text_centered(pixels, 116, "Drop a GB or GBC", gui_palette_native[3], gui_palette_native[0], false);
                    draw_text_centered(pixels, 128, "file to play", gui_palette_native[3], gui_palette_native[0], false);
                    break;
                case SHOWING_MENU:
                    draw_text_centered(pixels, 8, "SameBoy", gui_palette_native[3], gui_palette_native[0], false);
                    unsigned i = 0, y = 24;
                    for (const struct menu_item *item = current_menu; item->string; item++, i++) {
                        if (item->value_getter && !item->backwards_handler) {
                            char line[25];
                            snprintf(line, sizeof(line), "%s%*s", item->string, 24 - (int)strlen(item->string), item->value_getter(i));
                            draw_text_centered(pixels, y, line, gui_palette_native[3], gui_palette_native[0],
                                               i == current_selection ? DECORATION_SELECTION : DECORATION_NONE);
                            y += 12;
                            
                        }
                        else {
                            draw_text_centered(pixels, y, item->string, gui_palette_native[3], gui_palette_native[0],
                                               i == current_selection && !item->value_getter ? DECORATION_SELECTION : DECORATION_NONE);
                            y += 12;
                            if (item->value_getter) {
                                draw_text_centered(pixels, y, item->value_getter(i), gui_palette_native[3], gui_palette_native[0],
                                                   i == current_selection ? DECORATION_ARROWS : DECORATION_NONE);
                                y += 12;
                            }
                        }
                    }
                    break;
                case SHOWING_HELP:
                    draw_text(pixels, 2, 2, help[current_help_page], gui_palette_native[3], gui_palette_native[0]);
                    break;
                case WAITING_FOR_KEY:
                    draw_text_centered(pixels, 68, "Press a Key", gui_palette_native[3], gui_palette_native[0], DECORATION_NONE);
                    break;
            }
            
            SDL_UpdateTexture(texture, NULL, pixels, 160 * sizeof (uint32_t));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    } while (SDL_WaitEvent(&event));
}
