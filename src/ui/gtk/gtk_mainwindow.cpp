const char about_text[4096] = ""
  "bsnes -- version " BSNES_VERSION "\r\n"
  "Author: byuu\r\n"
  "Project began: October 14th, 2004\r\n"
  "\r\n\r\n"
  "Contributors:\r\n"
  "   anomie, blargg, DMV27, GIGO, kode54, Nach,\r\n"
  "   Overload, Richard Bannister, TRAC, zones";

struct MainWindow {
  GtkWidget *window;
  GtkWidget *box;

  GtkWidget *menu;
    GtkWidget *menu_file, *menu_file_item;
      GtkWidget *menu_file_loadrom;
      GtkWidget *menu_file_unloadrom;
      GtkWidget *menu_file_separator1;
      GtkWidget *menu_file_reset;
      GtkWidget *menu_file_power;
      GtkWidget *menu_file_separator2;
      GtkWidget *menu_file_quit;
    GtkWidget *menu_help, *menu_help_item;
      GtkWidget *menu_help_about;

  GdkColor   render_bg;
  GtkWidget *render;

  GtkWidget *fileopen;
  GtkWidget *aboutbox;
} main_window;

void init_main_window();
void menu_main_window(const char *item);
void term_main_window();
void load_rom();
void cancel_load_rom();
void unload_rom();
void snes_reset();
void snes_power();
void close_aboutbox();

void init_main_window() {
MainWindow *w = &main_window;

//
//create window
//
  w->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(w->window), BSNES_TITLE);
  gtk_window_set_resizable(GTK_WINDOW(w->window), false);
  g_signal_connect(G_OBJECT(w->window), "destroy", G_CALLBACK(term_main_window), 0);

//
//create widget container
//
  w->box = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(w->window), w->box);
  gtk_widget_show(w->box);

//
//create menu
//
  w->menu = gtk_menu_bar_new();
  gtk_widget_show(w->menu);

  w->menu_file = gtk_menu_new();
  w->menu_file_item = gtk_menu_item_new_with_label("File");
  gtk_widget_show(w->menu_file_item);

#define add_item(menu, item, label, id) \
  item = gtk_menu_item_new_with_label(label); \
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item); \
  g_signal_connect_swapped(G_OBJECT(item), "activate", G_CALLBACK(menu_main_window), (gpointer)id); \
  gtk_widget_show(item)
#define add_separator(menu, item) \
  item = gtk_separator_menu_item_new(); \
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item); \
  gtk_widget_show(item)

  add_item     (w->menu_file, w->menu_file_loadrom,   "Load ROM", "file.loadrom");
  add_item     (w->menu_file, w->menu_file_unloadrom, "Unload ROM", "file.unloadrom");
  add_separator(w->menu_file, w->menu_file_separator1);
  add_item     (w->menu_file, w->menu_file_reset,     "Reset", "file.reset");
  add_item     (w->menu_file, w->menu_file_power,     "Power (Hard Reset)", "file.power");
  add_separator(w->menu_file, w->menu_file_separator2);
  add_item     (w->menu_file, w->menu_file_quit,      "Quit", "file.quit");

  w->menu_help = gtk_menu_new();
  w->menu_help_item = gtk_menu_item_new_with_label("Help");
  gtk_widget_show(w->menu_help_item);

  add_item     (w->menu_help, w->menu_help_about,     "About", "help.about");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(w->menu_file_item), w->menu_file);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(w->menu_help_item), w->menu_help);

  gtk_menu_bar_append(GTK_MENU_BAR(w->menu), w->menu_file_item);
  gtk_menu_bar_append(GTK_MENU_BAR(w->menu), w->menu_help_item);

#undef add_item
#undef add_separator

//
//create render widget
//
  w->render_bg.pixel = 0;
  w->render_bg.red   = 0;
  w->render_bg.green = 0;
  w->render_bg.blue  = 0;

  w->render = gtk_drawing_area_new();
  gtk_widget_set_size_request(w->render, 256, 224);
  gtk_widget_modify_bg(w->render, GTK_STATE_NORMAL, &w->render_bg);
  gtk_widget_show(w->render);

//
//pack widgets
//
  gtk_box_pack_start(GTK_BOX(w->box), w->menu, false, false, 0);
  gtk_box_pack_end(GTK_BOX(w->box), w->render, true, true, 0);

//
//display window
//
  gtk_widget_show(w->window);

  while(gtk_events_pending() == true) {
    gtk_main_iteration_do(false);
  }

//
//initializations
//
  w->fileopen = 0;
}

void menu_main_window(const char *item) {
MainWindow *w = &main_window;

  if(!strcmp(item, "file.loadrom")) {
    if(!w->fileopen) {
      w->fileopen = gtk_file_selection_new("Load ROM");
      g_signal_connect_swapped(G_OBJECT(w->fileopen), "destroy", G_CALLBACK(cancel_load_rom), 0);
      g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(w->fileopen)->ok_button),     "clicked", G_CALLBACK(load_rom), 0);
      g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(w->fileopen)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer)w->fileopen);
    }
    gtk_widget_show(w->fileopen);
  } else if(!strcmp(item, "file.unloadrom")) {
    if(cartridge.loaded() == true) { cartridge.unload(); }
    gtk_window_set_title(GTK_WINDOW(w->window), BSNES_TITLE);
    gdk_window_invalidate_rect(GDK_WINDOW(w->render->window), 0, true);
  } else if(!strcmp(item, "file.reset")) {
    bsnes->reset();
  } else if(!strcmp(item, "file.power")) {
    bsnes->power();
  } else if(!strcmp(item, "file.quit")) {
    term_main_window();
  } else if(!strcmp(item, "help.about")) {
    if(!w->aboutbox) {
      w->aboutbox = gtk_message_dialog_new(0, (GtkDialogFlags)0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, about_text);
      gtk_window_set_title(GTK_WINDOW(w->aboutbox), "About bsnes...");
      g_signal_connect_swapped(G_OBJECT(w->aboutbox), "destroy",  G_CALLBACK(close_aboutbox), 0);
      g_signal_connect_swapped(G_OBJECT(w->aboutbox), "response", G_CALLBACK(gtk_widget_destroy), (gpointer)w->aboutbox);
    }
    gtk_widget_show(w->aboutbox);
  }
}

void term_main_window() {
  running = false;
}

void load_rom() {
  if(!main_window.fileopen)return;

const char *fn = gtk_file_selection_get_filename(GTK_FILE_SELECTION(main_window.fileopen));
  gtk_widget_hide(main_window.fileopen);

  if(cartridge.loaded() == true)  { cartridge.unload(); }
  if(cartridge.load(fn) == false) { return; }

  bsnes->power();
}

void cancel_load_rom() {
  main_window.fileopen = 0;
}

void close_aboutbox() {
  main_window.aboutbox = 0;
}
