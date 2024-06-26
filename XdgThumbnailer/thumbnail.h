#pragma once

#include <glib.h>
#include <gio/gio.h>

void load_boot_roms(void);
void unload_boot_roms(void);

void start_thumbnailing(unsigned handle, GCancellable *cancellable, gboolean is_urgent,
                        char const *uri, char const *mime_type);
