#include "hiro.hpp"
#include "port.cpp"

#include <nall/algorithm.hpp>
using nall::min;
using nall::max;

namespace libhiro {

static void set_font(GtkWidget *widget, gpointer font) {
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)set_font, font);
  }
}

static void set_default_font(GtkWidget *widget) {
  set_font(widget, phiro().font);
}

#include "keymap.cpp"
#include "widget.cpp"
  #include "window.cpp"
  #include "menucontrol.cpp"
    #include "menugroup.cpp"
    #include "menuitem.cpp"
    #include "menucheckitem.cpp"
    #include "menuradioitem.cpp"
    #include "menuseparator.cpp"
  #include "formcontrol.cpp"
    #include "frame.cpp"
    #include "canvas.cpp"
    #include "label.cpp"
    #include "button.cpp"
    #include "checkbox.cpp"
    #include "radiobox.cpp"
    #include "editbox.cpp"
    #include "listbox.cpp"
    #include "combobox.cpp"
    #include "progressbar.cpp"
    #include "slider.cpp"

void pHiro::init() {
  is_composited = false;
  *default_path = 0;
  screen = gdk_screen_get_default();
  if(gdk_screen_is_composited(screen)) {
    colormap = gdk_screen_get_rgba_colormap(screen);
    if(colormap) is_composited = true;
    else colormap = gdk_screen_get_rgb_colormap(screen); //fallback
  } else {
    colormap = gdk_screen_get_rgb_colormap(screen);
  }

  font = pango_font_description_new();
  pango_font_description_set_family(font, "Sans");
  pango_font_description_set_absolute_size(font, 11.0 * PANGO_SCALE);
  pango_font_description_set_style(font, PANGO_STYLE_NORMAL);

  //apply custom GTK+-2.0 stylesheet.
  //it's obviously not ideal to override the global GTK+ theme settings;
  //however it is necessary to ensure consistency between ports of hiro.
  //without this, it would be impossible to develop a hiro application
  //on one platform, and be assured text wouldn't clipped off, etc on
  //another platform.
  gtk_rc_parse_string(
  "style \"ruby-gtk\"\n"
  "{\n"
  "  GtkComboBox::appears-as-list = 1\n"     //text tends to get cut off in some themes otherwise
  "  GtkTreeView::vertical-separator = 0\n"  //GTK+ lists tend to have way more space than on Windows
  "}\n"
  "\n"
  "class \"GtkComboBox\" style \"ruby-gtk\"\n"
  "class \"GtkTreeView\" style \"ruby-gtk\"\n"
  );
}

void pHiro::term() {
  pango_font_description_free(font);
  enable_screensaver();
}

bool pHiro::run() {
  if(is_screensaver_enabled == false) screensaver_tick();
  gtk_main_iteration_do(false);
  return pending();
}

bool pHiro::pending() {
  return gtk_events_pending();
}

bool pHiro::folder_select(Window *focus, char *filename, const char *path) {
  if(!filename) return false;
  strcpy(filename, "");

  GtkWidget *dialog = gtk_file_chooser_dialog_new("Select Folder",
    focus ? GTK_WINDOW(focus->p.gtk_handle()) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0);

  if(path && *path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  else if(*default_path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_path);

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    set_default_path(fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename exists
}

bool pHiro::file_open(Window *focus, char *filename, const char *path, const char *filter) {
  if(!filename) return false;
  strcpy(filename, "");

  GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
    focus ? GTK_WINDOW(focus->p.gtk_handle()) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
    (const gchar*)0);

  if(path && *path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  else if(*default_path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_path);

  if(filter && *filter) {
    lstring filterlist;
    split(filterlist, "\n", filter);
    for(unsigned i = 0; i < count(filterlist); i++) {
      GtkFileFilter *filter = gtk_file_filter_new();
      lstring filterpart;
      split(filterpart, "\t", filterlist[i]);
      gtk_file_filter_set_name(filter, string() << filterpart[0] << " (" << filterpart[1] << ")");
      lstring patternlist;
      split(patternlist, ",", filterpart[1]);
      for(unsigned l = 0; l < count(patternlist); l++) {
        gtk_file_filter_add_pattern(filter, patternlist[l]);
      }
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    }
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    set_default_path(fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename exists
}

bool pHiro::file_save(Window *focus, char *filename, const char *path, const char *filter) {
  if(!filename) return false;
  strcpy(filename, "");

  GtkWidget *dialog = gtk_file_chooser_dialog_new("Save File",
    focus ? GTK_WINDOW(focus->p.gtk_handle()) : (GtkWindow*)0,
    GTK_FILE_CHOOSER_ACTION_SAVE,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
    GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
    (const gchar*)0);

  if(path && *path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
  else if(*default_path) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_path);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

  if(filter && *filter) {
    lstring filterlist;
    split(filterlist, "\n", filter);
    for(unsigned i = 0; i < count(filterlist); i++) {
      GtkFileFilter *filter = gtk_file_filter_new();
      lstring filterpart;
      split(filterpart, "\t", filterlist[i]);
      gtk_file_filter_set_name(filter, string() << filterpart[0] << " (" << filterpart[1] << ")");
      lstring patternlist;
      split(patternlist, ",", filterpart[1]);
      for(unsigned l = 0; l < count(patternlist); l++) {
        gtk_file_filter_add_pattern(filter, patternlist[l]);
      }
      gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    }
  }

  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    strcpy(filename, fn);
    set_default_path(fn);
    g_free(fn);
  }

  gtk_widget_destroy(dialog);
  return strcmp(filename, ""); //return true if filename exists
}

unsigned pHiro::screen_width() {
  return gdk_screen_width();
}

unsigned pHiro::screen_height() {
  return gdk_screen_height();
}

void pHiro::enable_screensaver() {
  if(is_screensaver_enabled == true) return;
  is_screensaver_enabled = true;
  DPMSDisable(GDK_DISPLAY());
}

void pHiro::disable_screensaver() {
  if(is_screensaver_enabled == false) return;
  is_screensaver_enabled = false;
  DPMSEnable(GDK_DISPLAY());
}

pHiro& pHiro::handle() {
  return hiro().p;
}

pHiro::pHiro(Hiro &self_) : self(self_) {
  is_screensaver_enabled = true;
}

pHiro& phiro() {
  return pHiro::handle();
}

/* internal */

//GTK+ does not save the most recent path to a file.
//Strip trailing filename / folder to save path for next file dialog request.
//This is only called when file dialog filename / folder is accepted, not when dialog cancelled.
void pHiro::set_default_path(const char *p) {
  strcpy(default_path, p);
  for(int i = strlen(default_path) - 1; i >= 0; i--) {
    if(default_path[i] == '/' || default_path[i] == '\\') {
      default_path[i] = 0;
      break;
    }
  }
}

void pHiro::screensaver_tick() {
  static clock_t delta_x = 0, delta_y = 0;

  delta_y = clock();
  if(delta_y - delta_x < CLOCKS_PER_SEC * 20) return;

  //XSetScreenSaver(timeout = 0) does not work
  //XResetScreenSaver() does not work
  //XScreenSaverSuspend() does not work
  //DPMSDisable() does not work
  //XSendEvent(KeyPressMask) does not work
  //use XTest extension to send fake keypress every ~20 seconds.
  //keycode of 255 does not map to any actual key, but it will block screensaver.
  delta_x = delta_y;
  XTestFakeKeyEvent(GDK_DISPLAY(), 255, True, 0);
  XSync(GDK_DISPLAY(), False);
  XTestFakeKeyEvent(GDK_DISPLAY(), 255, False, 0);
  XSync(GDK_DISPLAY(), False);
}

} //namespace libhiro
