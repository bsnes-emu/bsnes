#include "emulate.h"

#include <glib.h>
#include <gio/gio.h>
#include <stdint.h>

#include "Core/gb.h"

// Auto-generated via `glib-compile-resources` from `resources.gresource.xml`.
#include "build/obj/XdgThumbnailer/resources.h"

#define NB_FRAMES_TO_EMULATE (60 * 10)

#define BOOT_ROM_SIZE (0x100 + 0x800) // The two "parts" of it, which are stored contiguously.
static char *boot_rom;

void load_boot_rom(void)
{
    static char const *boot_rom_path = DATA_DIR "/cgb_boot_fast.bin";

    size_t length;
    GError *error = NULL;
    g_file_get_contents(boot_rom_path, &boot_rom, &length, &error);

    if (error) {
        g_error("Error loading boot ROM from \"%s\": %s", boot_rom_path, error->message);
        // NOTREACHED
    }
    else if (length != BOOT_ROM_SIZE) {
        g_error("Error loading boot ROM from \"%s\": expected to read %d bytes, got %zu",
                boot_rom_path, BOOT_ROM_SIZE, length);
        // NOTREACHED
    }
}

void unload_boot_rom(void) { g_free(boot_rom); }

/* --- */

static char *async_input_callback(GB_gameboy_t *gb)
{
    return NULL;
}

static void log_callback(GB_gameboy_t *gb, const char *string, GB_log_attributes attributes)
{
    // Swallow any logs.
}

static void vblank_callback(GB_gameboy_t *gb, GB_vblank_type_t type)
{
    unsigned nb_frames_left = GPOINTER_TO_UINT(GB_get_user_data(gb));
    nb_frames_left--;
    GB_set_user_data(gb, GUINT_TO_POINTER(nb_frames_left));

    // *Do* render the very last frame.
    if (nb_frames_left == 0) {
        GB_set_rendering_disabled(gb, false);
    }
}

static uint32_t rgb_encode(GB_gameboy_t *gb, uint8_t r, uint8_t g, uint8_t b)
{
    return r | g << 8 | b << 16 | 0xFF << 24;
}

unsigned emulate(enum FileKind kind, unsigned char const *rom, size_t rom_size, uint32_t screen[static 160 * 144])
{
    GB_gameboy_t gb;
    GB_init(&gb, GB_MODEL_CGB_E);

    GError *error = NULL;
    GBytes *boot_rom = g_resource_lookup_data(resources_get_resource(), "/thumbnailer/cgb_boot_fast.bin", G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    g_assert_no_error(error);
    size_t boot_rom_size;
    unsigned char const *boot_rom_data = g_bytes_get_data(boot_rom, &boot_rom_size);
    g_assert_cmpuint(boot_rom_size, ==, BOOT_ROM_SIZE);
    GB_load_boot_rom_from_buffer(&gb, boot_rom_data, boot_rom_size);
    g_bytes_unref(boot_rom);

    if (kind == KIND_ISX) {
        g_assert_not_reached(); // TODO: implement GB_load_isx_from_buffer
    }
    else {
        GB_load_rom_from_buffer(&gb, rom, rom_size);
    }

    GB_set_user_data(&gb, GUINT_TO_POINTER(NB_FRAMES_TO_EMULATE));

    GB_set_vblank_callback(&gb, vblank_callback);
    GB_set_pixels_output(&gb, screen);
    GB_set_rgb_encode_callback(&gb, rgb_encode);
    GB_set_async_input_callback(&gb, async_input_callback);
    GB_set_log_callback(&gb, log_callback);
    GB_set_color_correction_mode(&gb, GB_COLOR_CORRECTION_MODERN_BALANCED);

    GB_set_rendering_disabled(&gb, true);
    GB_set_turbo_mode(&gb, true, true);
    while (GPOINTER_TO_UINT(GB_get_user_data(&gb))) {
        GB_run(&gb);
    }

    unsigned cgb_flag = GB_read_memory(&gb, 0x143) & 0xC0;
    GB_free(&gb);
    return cgb_flag;
}
