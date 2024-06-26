#pragma once

#include <glib.h>
#include <gio/gio.h>

void init_tasks(void);
void cleanup_tasks(void);

struct NewTaskInfo {
    unsigned handle;
    GCancellable *cancellable;
};
struct NewTaskInfo new_task(gboolean is_urgent);
void cancel_task(unsigned handle);
void finished_task(unsigned handle);
