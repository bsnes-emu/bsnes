#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

#define None X11None
#define Window X11Window

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>

#undef None
#undef Window

using namespace nall;

namespace phoenix {

#include "object.cpp"
#include "font.cpp"
#include "menu.cpp"
#include "widget.cpp"
#include "window.cpp"
#include "layout.cpp"
#include "button.cpp"
#include "canvas.cpp"
#include "checkbox.cpp"
#include "combobox.cpp"
#include "editbox.cpp"
#include "hexeditor.cpp"
#include "horizontalslider.cpp"
#include "label.cpp"
#include "listbox.cpp"
#include "progressbar.cpp"
#include "radiobox.cpp"
#include "textbox.cpp"
#include "verticalslider.cpp"
#include "viewport.cpp"
#include "messagewindow.cpp"

Window Window::None;

void OS::initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

  int argc = 1;
  char *argv[2];
  argv[0] = new char[8];
  argv[1] = 0;
  strcpy(argv[0], "phoenix");
  char **argvp = argv;
  gtk_init(&argc, &argvp);

  gtk_rc_parse_string(
    "style \"phoenix-gtk\"\n"
    "{\n"
    "  GtkComboBox::appears-as-list = 1\n"
    "  GtkTreeView::vertical-separator = 0\n"
    "}\n"
    "class \"GtkComboBox\" style \"phoenix-gtk\"\n"
    "class \"GtkTreeView\" style \"phoenix-gtk\"\n"
  );
}

bool OS::pending() {
  return gtk_events_pending();
}

void OS::run() {
  while(pending()) gtk_main_iteration_do(false);
}

void OS::main() {
  gtk_main();
}

void OS::quit() {
  gtk_main_quit();
}

unsigned OS::desktopWidth() {
  return gdk_screen_get_width(gdk_screen_get_default());
}

unsigned OS::desktopHeight() {
  return gdk_screen_get_height(gdk_screen_get_default());
}

string OS::folderSelect(Window &parent, const string &path) {
  string name;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "Select Folder",
    &parent != &Window::None ? GTK_WINDOW(parent.object->widget) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0
  );

  if(path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  if(name.endswith("/") == false) name.append("/");
  return name;
}

string OS::fileOpen(Window &parent, const string &filter, const string &path) {
  string name;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "Open File",
    &parent != &Window::None ? GTK_WINDOW(parent.object->widget) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0
  );

  if(path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  lstring list;
  list.split("\n", filter);
  foreach(item, list) {
    lstring part;
    part.split("\t", item);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, string(part[0], " (", part[1], ")"));
    lstring patterns;
    patterns.split(",", part[1]);
    foreach(pattern, patterns) gtk_file_filter_add_pattern(filter, pattern);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  return name;
}

string OS::fileSave(Window &parent, const string &filter, const string &path) {
  string name;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
    "Save File",
    &parent != &Window::None ? GTK_WINDOW(parent.object->widget) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_SAVE,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0
  );

  if(path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  lstring list;
  list.split("\n", filter);
  foreach(item, list) {
    lstring part;
    part.split("\t", item);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, string(part[0], " (", part[1], ")"));
    lstring patterns;
    patterns.split(",", part[1]);
    foreach(pattern, patterns) gtk_file_filter_add_pattern(filter, pattern);
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    name = temp;
    g_free(temp);
  }

  gtk_widget_destroy(dialog);
  return name;
}

}
