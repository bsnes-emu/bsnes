#include "thumbnail.h"

#include <fcntl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gio/gio.h>
#include <glib-unix.h>
#include <glib.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "emulate.h"
#include "glibconfig.h"
#include "main.h"
#include "tasks.h"

#define THUMBNAILING_ERROR_DOMAIN (g_quark_from_static_string("thumbnailing"))

static char const dmg_only_resource_path[] = "/thumbnailer/CartridgeTemplate.png";
static char const dual_resource_path[] = "/thumbnailer/UniversalCartridgeTemplate.png";
static char const cgb_only_resource_path[] = "/thumbnailer/ColorCartridgeTemplate.png";

static char const gb_mime_type[] = "application/x-gameboy-rom";
static char const gbc_mime_type[] = "application/x-gameboy-color-rom";
static char const isx_mime_type[] = "application/x-gameboy-isx";

/* --- */

enum ThumbnailFlavor {
    FLAVOR_NORMAL,   // 128×128.
    FLAVOR_LARGE,    // 256×256.
    FLAVOR_X_LARGE,  // 512×512.
    FLAVOR_XX_LARGE, // 1024×1024.
};

static char const flavor_dir_names[][9] = {
    [FLAVOR_NORMAL] = "normal",
    [FLAVOR_LARGE] = "large",
    [FLAVOR_X_LARGE] = "x-large",
    [FLAVOR_XX_LARGE] = "xx-large",
};

static GPathBuf thumbnail_dir_path;

static __attribute__((returns_nonnull)) char const *get_home_path(void)
{
    char const *home = g_getenv("HOME"); // Statically allocated, so no need to free.
    if (home != NULL) return home;

    // This is pushing it, but let's give it one last shot.
    uid_t uid = geteuid();
    struct passwd *passwd_entry;
    do {
        errno = 0;                    // It may be left alone if no such entry can be found.
        passwd_entry = getpwuid(uid); // Statically allocated, so no need to free.
    } while (!passwd_entry && errno == EINTR);
    if (passwd_entry == NULL) {
        g_error("`XDG_CACHE_HOME` is unset, and failed to get the path to user %ju's home: %s", (uintmax_t)uid,
                strerror(errno));
        // NOTREACHED
    }
    return passwd_entry->pw_dir;
}

void locate_and_create_thumbnail_dir(void)
{
    // Compute the path to the thumbnail directory.
    char const *cache_home = g_getenv("XDG_CACHE_HOME"); // Statically allocated, so no need to free.
    if (cache_home != NULL) {
        g_path_buf_init_from_path(&thumbnail_dir_path, cache_home);
    }
    else {
        char const *home = get_home_path();
        g_path_buf_init_from_path(&thumbnail_dir_path, home);
        g_path_buf_push(&thumbnail_dir_path, ".cache");
    }
    g_path_buf_push(&thumbnail_dir_path, "thumbnails");

    // Create the thumbnail directories if they don't already exist.
    for (size_t i = 0; i < G_N_ELEMENTS(flavor_dir_names); ++i) {
        g_path_buf_push(&thumbnail_dir_path, flavor_dir_names[i]);
        char *path = g_path_buf_to_path(&thumbnail_dir_path);

        if (g_mkdir_with_parents(path, S_IRWXU) != 0) { // The permissions are mandated by the thumbnail spec.
            g_error("Failed to create thumbnail cache directory at \"%s\": %s", path, strerror(errno));
            // NOTREACHED
        }

        g_free(path);
        // Restore the GPathBuf to its original state (pointing at the thumbnail cache's root dir).
        g_path_buf_pop(&thumbnail_dir_path);
    }
    // ...thus, we end with the path buf pointing to the root dir, which is what the rest of this module expects.
}

void free_thumbnail_dir_path(void) { g_path_buf_clear(&thumbnail_dir_path); }

/* --- */

struct TaskData {
    char *contents;
    size_t length;
    enum FileKind kind;
    enum ThumbnailFlavor flavor;
};

static void destroy_task_data(void *data)
{
    struct TaskData *task_data = data;

    g_free(task_data->contents);
    g_slice_free(struct TaskData, task_data);
}

__attribute__((returns_nonnull)) char const *mime_type(enum FileKind kind)
{
    switch (kind) {
        case KIND_GB:
            return gb_mime_type;
        case KIND_GBC:
            return gbc_mime_type;
        case KIND_ISX:
            return isx_mime_type;
    }
    __builtin_unreachable();
}

/* --- */

#define GB_SCREEN_WIDTH 160
#define GB_SCREEN_HEIGHT 144

static void generate_thumbnail(GTask *task, void *source_object, void *data, GCancellable *cancellable)
{
    struct TaskData *task_data = data;
    char const *uri = g_task_get_name(task);
    char *md5 = g_compute_checksum_for_string(G_CHECKSUM_MD5, uri, -1);

    uint32_t screen_raw[GB_SCREEN_WIDTH * GB_SCREEN_HEIGHT];
    unsigned cgb_flag =
        emulate(task_data->kind, (unsigned char const *)task_data->contents, task_data->length, screen_raw);

    // Generate the thumbnail from `screen_raw` and `cgb_flag`.

    // `screen_raw` is properly formatted for this operation; see the comment in `rgb_encode` for a
    // discussion of why and how.
    GdkPixbuf *screen = gdk_pixbuf_new_from_data((uint8_t *)screen_raw, GDK_COLORSPACE_RGB,
                                                 true,                                    // Yes, we have alpha!
                                                 8,                                       // bpp
                                                 GB_SCREEN_WIDTH, GB_SCREEN_HEIGHT,       // Size.
                                                 GB_SCREEN_WIDTH * sizeof(screen_raw[0]), // Row stride.
                                                 NULL, NULL);                             // Do not free the buffer.
    // Scale the screen and position it in the appropriate place for compositing the cartridge templates.
    GdkPixbuf *scaled_screen = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, 1024, 1024);
    gdk_pixbuf_scale(screen,                                    // Source.
                     scaled_screen,                             // Destination.
                     192, 298,                                  // Match the displacement below.
                     GB_SCREEN_WIDTH * 4, GB_SCREEN_HEIGHT * 4, // How the scaled rectangle should be cropped.
                     192, 298, // Displace the scaled screen so it lines up with the template.
                     4, 4,     // Scaling factors.
                     GDK_INTERP_NEAREST);
    g_object_unref(screen);

    GError *error = NULL;
    GdkPixbuf *template;
    switch (cgb_flag) {
        case 0xC0:
            template = gdk_pixbuf_new_from_resource(cgb_only_resource_path, &error);
            break;
        case 0x80:
            template = gdk_pixbuf_new_from_resource(dual_resource_path, &error);
            break;
        default:
            template = gdk_pixbuf_new_from_resource(dmg_only_resource_path, &error);
            break;
    }
    g_assert_no_error(error);
    g_assert_cmpint(gdk_pixbuf_get_width(template), ==, 1024);
    g_assert_cmpint(gdk_pixbuf_get_height(template), ==, 1024);
    gdk_pixbuf_composite(template,           // Source.
                         scaled_screen,      // Destination.
                         0, 0,               // Match the displacement below.
                         1024, 1024,         // Crop of the scaled rectangle.
                         0, 0,               // Displacement of the scaled rectangle.
                         1, 1,               // Scaling factors.
                         GDK_INTERP_NEAREST, // Doesn't really matter, but should be a little faster.
                         255);               // Blending factor of the source onto the destination.
    g_object_unref(template);

    GPathBuf *output_path_buf = g_path_buf_copy(&thumbnail_dir_path);
    g_path_buf_push(output_path_buf, flavor_dir_names[task_data->flavor]);
    g_assert_cmpuint(strlen(md5), ==, 32);
    // The buffer's size is checked by the compiler (`-Wformat-overflow=2`).
    char temp_file_name[1 + 32 + 1 + 20 + 1 + 10 + 4 + 1]; // (".%s_%lu_%u.png", md5, pid, handle).
    temp_file_name[0] = '.';
    memcpy(&temp_file_name[1], md5, 32);
    sprintf(&temp_file_name[1 + 32], "_%lu_%u.png",
            pid + 0lu, // Promote the `pid_t` to `long int` if possible; `-Wformat` will warn otherwise.
            GPOINTER_TO_UINT(g_task_get_task_data(task)));
    g_path_buf_push(output_path_buf, temp_file_name);
    char *temp_file_path = g_path_buf_to_path(output_path_buf);

    g_debug("Saving pixel buf for \"%s\" to \"%s\"", g_task_get_name(task), temp_file_path);
    char file_size[sizeof(G_STRINGIFY(SIZE_MAX))];
    sprintf(file_size, "%zu", task_data->length);
    gdk_pixbuf_save(scaled_screen, temp_file_path, "png", &error, // "Base" parameters.
                    "tEXt::Thumb::URI", uri,                         // URI of the file being thumbnailed.
                    // "tEXt::Thumb::MTime", "", // TODO
                    "tEXt::Thumb::Size", file_size, // Size (in bytes) of the file being thumbnailed.
                    "tEXt::Thumb::Mimetype", mime_type(task_data->kind), // MIME type of the file being thumbnailed.
                    NULL);
    g_object_unref(scaled_screen);
    if (error) {
        g_path_buf_free(output_path_buf);
        g_free(temp_file_path);

        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_COULD_NOT_WRITE, "Failed to write image: %s", error->message);
        g_error_free(error);
        g_object_unref(task);
        return;
    }

    // Trim off all of the "temporary" parts.
    memcpy(&temp_file_name[1 + 32], ".png", sizeof(".png"));
    g_path_buf_set_filename(output_path_buf, &temp_file_name[1]);
    char *output_path = g_path_buf_free_to_path(output_path_buf);
    g_debug("Moving thumbnail for \"%s\" from \"%s\" to \"%s\"", g_task_get_name(task), temp_file_path, output_path);
    int rename_ret = rename(temp_file_path, output_path);
    int rename_errno = errno;
    g_free(temp_file_path);
    g_free(output_path);
    if (rename_ret != 0) {
        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_COULD_NOT_WRITE, "Failed to rename image from temp path: %s", strerror(rename_errno));
        g_object_unref(task);
        return;
    }

    g_task_return_boolean(task, true);
    g_object_unref(task);
}

// Callback when an async file operation completes.
static void on_file_ready(GObject *source_object, GAsyncResult *res, void *user_data)
{
    GFile *file = G_FILE(source_object);
    GTask *task = user_data;
    char const *uri = g_task_get_name(task);
    g_debug("File \"%s\" is done being read", uri);
    struct TaskData *task_data = g_task_get_task_data(task);

    GError *error = NULL;
    g_file_load_contents_finish(file, res, &task_data->contents, &task_data->length, NULL, &error);
    g_object_unref(file);

    if (error) {
        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_UNKNOWN_SCHEME_OR_MIME,
                                "Failed to load URI \"%s\": %s", uri, error->message);
        g_error_free(error);
        g_object_unref(task);
        return;
    }

    if (g_task_return_error_if_cancelled(task)) {
        g_object_unref(task);
        return;
    }

    // TODO: cap the max number of active threads.
    g_task_run_in_thread(task, generate_thumbnail);
}

/* --- */

static void on_thumbnailing_end(GObject *source_object, GAsyncResult *res, void *user_data)
{
    // TODO: start a new thread if some task is pending.

    g_assert_null(source_object); // The object that was passed to `g_task_new`.
    GTask *task = G_TASK(res);
    g_info("Ending thumbnailing for \"%s\"", g_task_get_name(task));
    unsigned handle = GPOINTER_TO_UINT(user_data);
    char const *uri = g_task_get_name(task);

    GError *error = NULL;
    if (g_task_propagate_boolean(task, &error)) {
        g_signal_emit_by_name(thumbnailer_interface, "ready", handle, uri);
    }
    else if (!g_cancellable_is_cancelled(g_task_get_cancellable(task))) {
        // If the task was cancelled, do not emit an error response.
        g_signal_emit_by_name(thumbnailer_interface, "error", handle, uri, error->code, error->message);
    }
    g_signal_emit_by_name(thumbnailer_interface, "finished", handle);

    finished_task(handle);
}

void start_thumbnailing(unsigned handle, GCancellable *cancellable, gboolean is_urgent, char const *uri,
                        char const *mime_type, char const *flavor_name)
{
    g_signal_emit_by_name(thumbnailer_interface, "started", handle);

    GTask *task = g_task_new(NULL, cancellable, on_thumbnailing_end, GUINT_TO_POINTER(handle));
    g_task_set_priority(task, is_urgent ? G_PRIORITY_HIGH : G_PRIORITY_DEFAULT);
    g_task_set_name(task, uri);

    enum FileKind kind;
    if (g_strcmp0(mime_type, gbc_mime_type) == 0) {
        kind = KIND_GBC;
    }
    else if (g_strcmp0(mime_type, gb_mime_type) == 0) {
        kind = KIND_GB;
    }
    else if (g_strcmp0(mime_type, isx_mime_type) == 0) {
        kind = KIND_ISX;
    }
    else {
        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_UNKNOWN_SCHEME_OR_MIME,
                                "Unsupported MIME type \"%s\"", mime_type);
        g_object_unref(task);
        return;
    }

    enum ThumbnailFlavor flavor;
    if (g_strcmp0(flavor_name, "normal") == 0) {
        flavor = FLAVOR_NORMAL;
    }
    else if (g_strcmp0(flavor_name, "large") == 0) {
        flavor = FLAVOR_LARGE;
    }
    else if (g_strcmp0(flavor_name, "x-large") == 0) {
        flavor = FLAVOR_X_LARGE;
    }
    else if (g_strcmp0(flavor_name, "xx-large") == 0) {
        flavor = FLAVOR_XX_LARGE;
    }
    else {
        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_UNSUPPORTED_FLAVOR,
                                "Unsupported thumbnail size/flavor \"%s\"", flavor_name);
        g_object_unref(task);
        return;
    }

    struct TaskData *task_data = g_slice_new(struct TaskData);
    task_data->contents = NULL;
    task_data->kind = kind;
    task_data->flavor = flavor;
    g_task_set_task_data(task, task_data, destroy_task_data);

    GFile *file = g_file_new_for_uri(uri);
    g_file_load_contents_async(file, cancellable, on_file_ready, task);
}
