#include <stdbool.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include "gb.h"
#include "utils.h"
#include "gui.h"
#include "shader.h"

#ifndef _WIN32
#define AUDIO_FREQUENCY 96000
#else
/* Windows (well, at least my VM) can't handle 96KHz sound well :( */
#define AUDIO_FREQUENCY 44100
#endif

GB_gameboy_t gb;
static bool dmg = false;
static bool paused = false;
static uint32_t pixel_buffer_1[160*144], pixel_buffer_2[160*144];
static uint32_t *active_pixel_buffer = pixel_buffer_1, *previous_pixel_buffer = pixel_buffer_2;


static char *filename = NULL;
static bool should_free_filename = false;
static char *battery_save_path_ptr;

void set_filename(const char *new_filename, bool new_should_free)
{
    if (filename && should_free_filename) {
        SDL_free(filename);
    }
    filename = (char *) new_filename;
    should_free_filename = new_should_free;
}

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
                pending_command = GB_SDL_QUIT_COMMAND;
                break;
                
            case SDL_DROPFILE: {
                set_filename(event.drop.file, true);
                pending_command = GB_SDL_NEW_FILE_COMMAND;
                break;
            }
                
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    update_viewport();
                }
                break;
            }
                
            case SDL_JOYBUTTONUP:
            case SDL_JOYBUTTONDOWN:
                event.jbutton.button = fix_joypad_button(event.jbutton.button);
                if (event.jbutton.button < 4) {
                    GB_set_key_state(gb, (event.jbutton.button & 1) ? GB_KEY_A : GB_KEY_B,
                                     event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == 8) {
                    GB_set_key_state(gb, GB_KEY_SELECT, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == 9) {
                    GB_set_key_state(gb, GB_KEY_START, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                    GB_set_key_state(gb, GB_KEY_UP, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                    GB_set_key_state(gb, GB_KEY_DOWN, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                    GB_set_key_state(gb, GB_KEY_LEFT, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                    GB_set_key_state(gb, GB_KEY_RIGHT, event.type == SDL_JOYBUTTONDOWN);
                }
                else if (event.jbutton.button & 1) {
                    GB_set_turbo_mode(gb, event.type == SDL_JOYBUTTONDOWN, false);
                }
                
                else {
                    bool audio_playing = SDL_GetAudioStatus() == SDL_AUDIO_PLAYING;
                    if (audio_playing) {
                        SDL_PauseAudio(true);
                    }
                    run_gui(true);
                    if (audio_playing) {
                        SDL_PauseAudio(false);
                    }
                    GB_set_color_correction_mode(gb, configuration.color_correction_mode);
                    GB_set_highpass_filter_mode(gb, configuration.highpass_mode);
                }
            break;
                
            case SDL_JOYAXISMOTION:
                event.jaxis.axis = fix_joypad_axis(event.jaxis.axis);
                if (event.jaxis.axis == 1) {
                    GB_set_key_state(gb, GB_KEY_DOWN, event.jaxis.value > 0x4000);
                    GB_set_key_state(gb, GB_KEY_UP, event.jaxis.value < -0x4000);
                }
                else if (event.jaxis.axis == 0) {
                    GB_set_key_state(gb, GB_KEY_RIGHT, event.jaxis.value > 0x4000);
                    GB_set_key_state(gb, GB_KEY_LEFT, event.jaxis.value < -0x4000);
                }
            break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE: {
                        bool audio_playing = SDL_GetAudioStatus() == SDL_AUDIO_PLAYING;
                        if (audio_playing) {
                            SDL_PauseAudio(true);
                        }
                        run_gui(true);
                        if (audio_playing) {
                            SDL_PauseAudio(false);
                        }
                        GB_set_color_correction_mode(gb, configuration.color_correction_mode);
                        GB_set_highpass_filter_mode(gb, configuration.highpass_mode);
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
                        
                    case SDL_SCANCODE_T:
                        if (event.key.keysym.mod & MODIFIER) {
                            pending_command = GB_SDL_TOGGLE_MODEL_COMMAND;
                        }
                        break;
                    
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
                            SDL_PauseAudio(SDL_GetAudioStatus() == SDL_AUDIO_PLAYING? true : false);
                        }
                        break;
                        
                    default:
                        /* Save states */
                        if (event.key.keysym.scancode >= SDL_SCANCODE_0 && event.key.keysym.scancode <= SDL_SCANCODE_9) {
                            if (event.key.keysym.mod & MODIFIER) {
                                command_parameter = event.key.keysym.scancode - SDL_SCANCODE_0;
                                
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
                    GB_set_turbo_mode(gb, event.type == SDL_KEYDOWN, false);
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

static void vblank(GB_gameboy_t *gb)
{
    if (configuration.blend_frames) {
        render_texture(active_pixel_buffer, previous_pixel_buffer);
        uint32_t *temp = active_pixel_buffer;
        active_pixel_buffer = previous_pixel_buffer;
        previous_pixel_buffer = temp;
        GB_set_pixels_output(gb, active_pixel_buffer);
    }
    else {
        render_texture(active_pixel_buffer, NULL);
    }
    handle_events(gb);
}


static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(pixel_format, r, g, b);
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
            
        case GB_SDL_QUIT_COMMAND:
            GB_save_battery(&gb, battery_save_path_ptr);
            exit(0);
    }
    return false;
}

static void run(void)
{
    pending_command = GB_SDL_NO_COMMAND;
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
        GB_set_pixels_output(&gb, active_pixel_buffer);
        GB_set_rgb_encode_callback(&gb, rgb_encode);
        GB_set_sample_rate(&gb, have_aspec.freq);
        GB_set_color_correction_mode(&gb, configuration.color_correction_mode);
        GB_set_highpass_filter_mode(&gb, configuration.highpass_mode);
    }
    
    bool error = false;
    start_capturing_logs();
    if (dmg) {
        error = GB_load_boot_rom(&gb, executable_relative_path("dmg_boot.bin"));
    }
    else {
        error = GB_load_boot_rom(&gb, executable_relative_path("cgb_boot.bin"));
    }
    end_capturing_logs(true, error);
    
    start_capturing_logs();
    error = GB_load_rom(&gb, filename);
    end_capturing_logs(true, error);
    
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

static char prefs_path[1024] = {0, };

static void save_configuration(void)
{
    FILE *prefs_file = fopen(prefs_path, "wb");
    if (prefs_file) {
        fwrite(&configuration, 1, sizeof(configuration), prefs_file);
        fclose(prefs_file);
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
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("SameBoy v" xstr(VERSION), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              160 * 2, 144 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowMinimumSize(window, 160, 144);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    
    if (major * 0x100 + minor < 0x302) {
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

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    
    char *prefs_dir = SDL_GetPrefPath("", "SameBoy");
    snprintf(prefs_path, sizeof(prefs_path) - 1, "%sprefs.bin", prefs_dir);
    SDL_free(prefs_dir);
    
    FILE *prefs_file = fopen(prefs_path, "rb");
    if (prefs_file) {
        fread(&configuration, 1, sizeof(configuration), prefs_file);
        fclose(prefs_file);
    }
    
    atexit(save_configuration);
    
    if (!init_shader_with_name(&shader, configuration.filter)) {
        init_shader_with_name(&shader, "NearestNeighbor");
    }
    update_viewport();
    
    if (filename == NULL) {
        run_gui(false);
    }
    SDL_PauseAudio(false);
    run(); // Never returns
    return 0;
}
