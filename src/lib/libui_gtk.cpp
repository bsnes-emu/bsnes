#include "libui_gtk.h"
#include "libui_gtk_window.cpp"
#include "libui_gtk_control.cpp"

namespace libui {

void init() {
int argc = 1;
char **argv;
  argv = (char**)malloc(1 * sizeof(char*));
  argv[0] = (char*)malloc(64 * sizeof(char));
  strcpy(argv[0], "./libui");
//GTK+ insists you provide main()'s argc, argv parameters for
//some special command-line processing options that are likely
//never used by anything.
//However, I insist on libui_init() not requiring arguments so
//that it can be called from anywhere, including a global
//constructor that is invoked before main() is reached.
  gtk_init(&argc, &argv);
  safe_free(argv[0]);
  safe_free(argv);
}

void term() {
}

bool run() {
  gtk_main_iteration_do(false);
  return events_pending();
}

bool events_pending() {
  return gtk_events_pending();
}

uint get_screen_width()  { return gdk_screen_width();  }
uint get_screen_height() { return gdk_screen_height(); }

};
