#define G_LOG_DOMAIN "sameboy-thumbnailer"

#include <gio/gio.h>
#include <glib-object.h>
#include <glib-unix.h>
#include <glib.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

// Auto-generated via `gdbus-codegen` from `interface.xml`.
#include "build/obj/XdgThumbnailer/interface.h"

static char const *const name_on_bus = "com.github.liji32.sameboy.XdgThumbnailer";
static char const *const object_path = "/com/github/liji32/sameboy/XdgThumbnailer";

/* --- The main work being performed here --- */

static GThreadPool *thread_pool;

// The function called by the threads in `thread_pool`.
static void generate_thumbnail(void *data, void *user_data)
{
    // TODO
}

static gboolean handle_queue(ThumbnailerSpecializedThumbnailer1 *object,
                             GDBusMethodInvocation *invocation, char const *uri, char const *mime_type,
                             char const *flavor, gboolean urgent)
{
    g_info("Received Queue(uri=\"%s\", mime_type=\"%s\", flavor=\"%s\", urgent=%s) request", uri, mime_type, flavor, urgent ? "true" : "false");

    // TODO

    return G_DBUS_METHOD_INVOCATION_HANDLED;
}

static gboolean handle_dequeue(ThumbnailerSpecializedThumbnailer1 *object,
                               GDBusMethodInvocation *invocation, unsigned handle)
{
    g_info("Received Dequeue(handle=%u) request", handle);

    // TODO

    return G_DBUS_METHOD_INVOCATION_HANDLED;
}

/* --- "Glue"; or, how the above is orchestrated / wired up --- */

static GMainLoop *main_loop;

static void on_bus_acquired(GDBusConnection *connection, const char *name, void *user_data)
{
    g_assert_cmpstr(name, ==, name_on_bus);
    (void)user_data;
    g_info("Acquired bus");

    GError *error;

    // Create the interface, and hook up callbacks for when its methods are called.
    ThumbnailerSpecializedThumbnailer1 *thumbnailer_interface =
        thumbnailer_specialized_thumbnailer1_skeleton_new();
    g_signal_connect(thumbnailer_interface, "handle-queue", G_CALLBACK(handle_queue), NULL);
    g_signal_connect(thumbnailer_interface, "handle-dequeue", G_CALLBACK(handle_dequeue), NULL);

    // Export the interface on the bus.
    error = NULL;
    GDBusInterfaceSkeleton *interface = G_DBUS_INTERFACE_SKELETON(thumbnailer_interface);
    gboolean res = g_dbus_interface_skeleton_export(interface, connection, object_path, &error);
    g_assert(res);
    g_assert_no_error(error);
    if (error) {
        g_error("Error exporting interface \"%s\" at \"%s\": %s",
                g_dbus_interface_skeleton_get_info(interface)->name, object_path, error->message);
        // NOTREACHED
    }
}

static void on_name_acquired(GDBusConnection *connection, const char *name, void *user_data)
{
    g_assert_cmpstr(name, ==, name_on_bus);
    (void)user_data;

    g_info("Acquired the name \"%s\" on the session bus", name);
}

static void on_name_lost(GDBusConnection *connection, const char *name, void *user_data)
{
    g_assert_cmpstr(name, ==, name_on_bus);
    (void)user_data;

    if (connection != NULL) {
        g_info("Lost the name \"%s\" on the session bus", name);
    }
    else {
        g_error("Failed to connect to session bus");
    }
    g_main_loop_quit(main_loop);
}

static gboolean handle_sigterm(void *user_data)
{
    g_info("SIGTERM received! Quitting...");

    g_main_loop_quit(main_loop);
    return G_SOURCE_CONTINUE; // Do not remove this source ourselves, let the post-main loop do so.
}

int main(int argc, char const *argv[])
{
    GError *error;

    // Create the thread pool *before* starting to accept tasks from D-Bus.
    // Make it non-exclusive so that the number of spawned threads grows dynamically, to consume
    // fewer system resources when no thumbnails are being generated.
    thread_pool =
        g_thread_pool_new(generate_thumbnail, NULL, g_get_num_processors(), FALSE, &error);
    g_assert_no_error(error); // Creating a non-exclusive thread pool cannot generate errors.
    // Likewise, create the main loop before then, so it can be aborted even before entering it.
    main_loop = g_main_loop_new(NULL, FALSE);

    // Refuse to replace the name or be replaced; there should only be one instance of the
    // thumbnailer on the bus at all times. To replace this program, kill it.
    unsigned owner_id =
        g_bus_own_name(G_BUS_TYPE_SESSION, name_on_bus, G_BUS_NAME_OWNER_FLAGS_DO_NOT_QUEUE,
                       on_bus_acquired, on_name_acquired, on_name_lost, NULL, NULL);

    unsigned sigterm_source_id = g_unix_signal_add(SIGTERM, handle_sigterm, NULL);
    g_main_loop_run(main_loop);
    gboolean removed =
        g_source_remove(sigterm_source_id); // This must be done before destroying the main loop.
    g_assert(removed);

    g_info("Waiting for outstanding tasks...");
    g_thread_pool_free(thread_pool, FALSE, TRUE);
    g_main_loop_unref(main_loop);
    g_bus_unown_name(owner_id);
    return 0;
}
