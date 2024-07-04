#pragma once

#include <gio/gio.h>
#include <glib.h>

void locate_and_create_thumbnail_dir(void);
void free_thumbnail_dir_path(void);

void start_thumbnailing(unsigned handle, GCancellable *cancellable, gboolean is_urgent, char const *uri,
                        char const *mime_type, char const *flavor_name);
