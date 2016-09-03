#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define snprintf _snprintf
#endif

#include "gb.h"
#include "debugger.h"
#include "memory.h"

static bool running = false;
static char *filename;
static char *bmp_filename;
static char *log_filename;
static FILE *log_file;
static void replace_extension(const char *src, size_t length, char *dest, const char *ext);
static bool push_start_a;
static unsigned int test_length = 60 * 40;
GB_gameboy_t gb;

static unsigned int frames = 0;
const char bmp_header[] = {
0x42, 0x4D, 0x48, 0x68, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x38, 0x00,
0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x70, 0xFF,
0xFF, 0xFF, 0x01, 0x00, 0x20, 0x00, 0x03, 0x00,
0x00, 0x00, 0x02, 0x68, 0x01, 0x00, 0x12, 0x0B,
0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

uint32_t bitmap[160*144];

static void vblank(GB_gameboy_t *gb)
{
    /* Do not press any buttons during the last two seconds, this might cause a
       screenshot to be taken while the LCD is off if the press makes the game
       load graphics. */
    if (push_start_a && frames < test_length - 120) { 
        switch (frames % 40) {
            case 0:
                gb->keys[7] = true; // Start down
                break;
            case 10:
                gb->keys[7] = false; // Start up
                break;
            case 20:
                gb->keys[4] = true; // A down
                break;
            case 30:
                gb->keys[4] = false; // A up
                break;
        }
    }
    
    /* Detect common crashes and stop the test early */
    if (frames < test_length - 1) {
        if (gb->backtrace_size >= 0x80) {
            GB_log(gb, "A stack overflow has probably occurred.\n");
            frames = test_length - 1;
        }
        if (gb->pc == 0x38 && GB_read_memory(gb, 0x38) == 0xFF) {
            GB_log(gb, "The game is probably stuck in an FF loop.\n");
            frames = test_length - 1;
        }
        if (gb->halted && (!gb->ime || !gb->interrupt_enable)) {
            GB_log(gb, "The game is deadlocked.\n");
            frames = test_length - 1;
        }
    }

    if (frames == test_length) {
        FILE *f = fopen(bmp_filename, "wb");
        fwrite(&bmp_header, 1, sizeof(bmp_header), f);
        fwrite(&bitmap, 1, sizeof(bitmap), f);
        fclose(f);
        if (!gb->boot_rom_finished) {
            GB_log(gb, "Boot ROM did not finish.\n");
        }
        running = false;
    }
    else if (frames == test_length - 1) {
        gb->disable_rendering = false;
    }
    
    frames++;
}

static void log_callback(GB_gameboy_t *gb, const char *string, GB_log_attributes attributes)
{
    if (!log_file) log_file = fopen(log_filename, "w");
    fprintf(log_file, "%s", string);
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
    return (r << 24) | (g << 16) | (b << 8);
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
#define str(x) #x
#define xstr(x) str(x)
    fprintf(stderr, "SameBoy Tester v" xstr(VERSION) "\n");

    if (argc == 1) {
        fprintf(stderr, "Usage: %s [--dmg] [--start] [--length seconds]"
#ifndef _WIN32
                        " [--jobs number of tests to run simultaneously]"
#endif
                        " rom ...\n", argv[0]);
        exit(1);
    }

#ifndef _WIN32
    unsigned int max_forks = 1;
    unsigned int current_forks = 0;
#endif

    bool dmg = false;
    for (unsigned i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--dmg") == 0) {
            fprintf(stderr, "Using DMG mode\n");
            dmg = true;
            continue;
        }

        if (strcmp(argv[i], "--start") == 0) {
            fprintf(stderr, "Pushing Start and A\n");
            push_start_a = true;
            continue;
        }
        
        if (strcmp(argv[i], "--length") == 0 && i != argc - 1) {
            test_length = atoi(argv[++i]) * 60;
            fprintf(stderr, "Test length is %d seconds\n", test_length / 60);
            continue;
        }
        
#ifndef _WIN32
        if (strcmp(argv[i], "--jobs") == 0 && i != argc - 1) {
            max_forks = atoi(argv[++i]);
            /* Make sure wrong input doesn't blow anything up. */
            if (max_forks < 1) max_forks = 1;
            if (max_forks > 16) max_forks = 16;
            fprintf(stderr, "Running up to %d tests simultaneously\n", max_forks);
            continue;
        }

        if (max_forks > 1) {
            while (current_forks >= max_forks) {
                int wait_out;
                while(wait(&wait_out) == -1);
                current_forks--;
            }
            
            current_forks++;
            if (fork() != 0) continue;
        }
#endif
        filename = argv[i];
        size_t path_length = strlen(filename);

        char bitmap_path[path_length + 5]; /* At the worst case, size is strlen(path) + 4 bytes for .bmp + NULL */
        replace_extension(filename, path_length, bitmap_path, ".bmp");
        bmp_filename = &bitmap_path[0];
        
        char log_path[path_length + 5];
        replace_extension(filename, path_length, log_path, ".log");
        log_filename = &log_path[0];
        
        fprintf(stderr, "Testing ROM %s\n", filename);
        
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
        
        GB_set_vblank_callback(&gb, (GB_vblank_callback_t) vblank);
        GB_set_pixels_output(&gb, &bitmap[0]);
        GB_set_rgb_encode_callback(&gb, rgb_encode);
        GB_set_log_callback(&gb, log_callback);
        
        if (GB_load_rom(&gb, filename)) {
            perror("Failed to load ROM");
            exit(1);
        }

        /* Run emulation */
        running = true;
        gb.turbo = gb.turbo_dont_skip = gb.disable_rendering = true;
        frames = 0;
        while (running) {
            GB_run(&gb);
        }
        
        if (log_file) {
            fclose(log_file);
            log_file = NULL;
        }
        
        GB_free(&gb);
#ifndef _WIN32
        if (max_forks > 1) {
            exit(0);
        }
#endif
    }
#ifndef _WIN32
    int wait_out;
    while(wait(&wait_out) != -1);
#endif
    return 0;
}

