#include "miu.gtk.h"
#include "../miu.cpp"

namespace ns_miu {

#include "miu.gtk.keymap.cpp"
#include "miu.gtk.widget.cpp"
  #include "miu.gtk.window.cpp"
  #include "miu.gtk.menucontrol.cpp"
    #include "miu.gtk.menugroup.cpp"
    #include "miu.gtk.menuitem.cpp"
    #include "miu.gtk.menucheckitem.cpp"
    #include "miu.gtk.menuradioitem.cpp"
    #include "miu.gtk.menuseparator.cpp"
  #include "miu.gtk.formcontrol.cpp"
    #include "miu.gtk.frame.cpp"
    #include "miu.gtk.canvas.cpp"
    #include "miu.gtk.label.cpp"
    #include "miu.gtk.button.cpp"
    #include "miu.gtk.checkbox.cpp"
    #include "miu.gtk.radiobox.cpp"
    #include "miu.gtk.editbox.cpp"
    #include "miu.gtk.listbox.cpp"
    #include "miu.gtk.combobox.cpp"
    #include "miu.gtk.progressbar.cpp"
    #include "miu.gtk.slider.cpp"

void pMiu::init() {
//simulate passing argc, argv to gtk_init()
int argc = 1;
char **argv;
  argv = (char**)malloc(1 * sizeof(char*));
  argv[0] = (char*)malloc(64 * sizeof(char));
  strcpy(argv[0], "./miu");
  gtk_init(&argc, &argv);
  safe_free(argv[0]);
  safe_free(argv);
}

void pMiu::term() {
}

bool pMiu::run() {
  gtk_main_iteration_do(false);
  return pending();
}

bool pMiu::pending() {
  return gtk_events_pending();
}

bool pMiu::file_load(Window *focus, char *filename, const char *filter, const char *path) {
  if(!filename) return false;
  strcpy(filename, "");

GtkWidget *dialog = gtk_file_chooser_dialog_new("Load File",
  focus ? GTK_WINDOW(focus->p.gtk_handle()) : (GtkWindow*)0,
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
  (const gchar*)0);

  if(path && *path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
  char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename exists
}

bool pMiu::file_save(Window *focus, char *filename, const char *filter, const char *path) {
  if(!filename) return false;
  strcpy(filename, "");

GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
  focus ? GTK_WINDOW(focus->p.gtk_handle()) : (GtkWindow*)0,
  GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
  (const gchar*)0);

  if(path && *path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
  char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename exists
}

uint pMiu::screen_width() {
  return gdk_screen_width();
}

uint pMiu::screen_height() {
  return gdk_screen_height();
}

pMiu& pMiu::handle() {
  return miu().p;
}

pMiu::pMiu(Miu &self_) : self(self_) {
}

pMiu& pmiu() {
  return pMiu::handle();
}

} //namespace ns_miu
