#include "thumbnail.h"

#include <gio/gio.h>
#include <glib.h>
#include <stdint.h>
#include <stdlib.h>

#include "emulate.h"
#include "main.h"
#include "tasks.h"

#define THUMBNAILING_ERROR_DOMAIN (g_quark_from_static_string("thumbnailing"))

struct TaskData {
    char *contents;
    size_t length;
    enum FileKind kind;
};

static void destroy_task_data(void *data)
{
    struct TaskData *task_data = data;

    g_free(task_data->contents);
    g_slice_free(struct TaskData, task_data);
}

static void generate_thumbnail(GTask *task, void *source_object, void *data,
                               GCancellable *cancellable)
{
    struct TaskData *task_data = data;

    uint32_t screen[160 * 144];
    unsigned cgb_flag = emulate(task_data->kind, (unsigned char const *)task_data->contents,
                                task_data->length, screen);
    // TODO: generate the thumbnail from `screen` and `cgb_flag`.
    (void)cgb_flag;

    g_task_return_boolean(task, TRUE);
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
        g_signal_emit_by_name(thumbnailer_interface, "error", handle, uri, error->code,
                              error->message);
    }
    g_signal_emit_by_name(thumbnailer_interface, "finished", handle);

    finished_task(handle);
}

void start_thumbnailing(unsigned handle, GCancellable *cancellable, gboolean is_urgent,
                        char const *uri, char const *mime_type)
{
    g_signal_emit_by_name(thumbnailer_interface, "started", handle);

    GTask *task = g_task_new(NULL, cancellable, on_thumbnailing_end, GUINT_TO_POINTER(handle));
    g_task_set_priority(task, is_urgent ? G_PRIORITY_HIGH : G_PRIORITY_DEFAULT);
    g_task_set_name(task, uri);

    enum FileKind kind;
    if (g_strcmp0(mime_type, "application/x-gameboy-color-rom") == 0) {
        kind = KIND_GBC;
    }
    else if (g_strcmp0(mime_type, "application/x-gameboy-rom") == 0) {
        kind = KIND_GB;
    }
    else if (g_strcmp0(mime_type, "application/x-gameboy-isx") == 0) {
        kind = KIND_ISX;
    }
    else {
        g_task_return_new_error(task, THUMBNAILING_ERROR_DOMAIN, ERROR_UNKNOWN_SCHEME_OR_MIME,
                                "Unsupported MIME type %s", mime_type);
        g_object_unref(task);
        return;
    }

    struct TaskData *task_data = g_slice_new(struct TaskData);
    task_data->contents = NULL;
    task_data->kind = kind;
    g_task_set_task_data(task, task_data, destroy_task_data);

    GFile *file = g_file_new_for_uri(uri);
    g_file_load_contents_async(file, cancellable, on_file_ready, task);
}
