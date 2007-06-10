namespace libui {

gint libui_window_close(GtkWidget *w, GdkEventAny *any, Window *window) {
  if(window) { return !window->message(Message::Close); }
  return FALSE; //destroy window by default
}

gint libui_window_keydown(GtkWidget *w, GdkEventKey *key, Window *window) {
  if(window) { window->message(Message::KeyDown, libui::translate_key(key->keyval)); }
  return FALSE;
}

gint libui_window_keyup(GtkWidget *w, GdkEventKey *key, Window *window) {
  if(window) { window->message(Message::KeyUp, libui::translate_key(key->keyval)); }
  return FALSE;
}

void libui_control_clicked(Control *control) {
  if(control && control->owner) { control->owner->message(Message::Clicked, (uintptr_t)control); }
}

void libui_control_changed(Control *control) {
  if(control && control->owner) { control->owner->message(Message::Changed, (uintptr_t)control); }
}

void libui_control_double_clicked(Control *control) {
  if(control && control->owner) { control->owner->message(Message::DoubleClicked, (uintptr_t)control); }
}

void Window::create(uint style, uint width, uint height, const char *caption) {
  info.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(info.window), caption ? caption : "");
  gtk_window_set_resizable(GTK_WINDOW(info.window), false);
  g_signal_connect(G_OBJECT(info.window), "delete_event",      G_CALLBACK(libui_window_close),   (gpointer)this);
  g_signal_connect(G_OBJECT(info.window), "key_press_event",   G_CALLBACK(libui_window_keydown), (gpointer)this);
  g_signal_connect(G_OBJECT(info.window), "key_release_event", G_CALLBACK(libui_window_keyup),   (gpointer)this);

  if(style & Center) { gtk_window_set_position(GTK_WINDOW(info.window), GTK_WIN_POS_CENTER_ALWAYS); }

  info.vcontainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(info.window), info.vcontainer);
  gtk_widget_show(info.vcontainer);

//always create menubar, only display it when MenuBar type is created
//this is needed to setup box packing before menubar is defined
  info.menubar = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(info.vcontainer), info.menubar, false, false, 0);

  info.container = gtk_fixed_new();
  gtk_widget_set_size_request(info.container, width, height);
  gtk_box_pack_end(GTK_BOX(info.vcontainer), info.container, true, true, 0);
  gtk_widget_show(info.container);
}

void Window::focus() {
  gtk_window_present(GTK_WINDOW(info.window));
}

bool Window::focused() {
  return GTK_WIDGET_HAS_FOCUS(info.window);
}

void Window::move(uint x, uint y) {
//if window was centered before, GTK+ will ignore move requests,
//therfore we must turn off auto-centering.
  gtk_window_set_position(GTK_WINDOW(info.window), GTK_WIN_POS_NONE);
  gtk_window_move(GTK_WINDOW(info.window), x, y);
}

void Window::resize(uint width, uint height) {
  gtk_widget_set_size_request(info.container, width, height);
}

void Window::show() {
  gtk_widget_show(info.window);
}

void Window::hide() {
  gtk_widget_hide(info.window);
}

void Window::show(bool state) {
  (state == true) ? show() : hide();
}

bool Window::visible() {
  return GTK_WIDGET_VISIBLE(info.window);
}

void Window::fullscreen() {
  gtk_window_fullscreen(GTK_WINDOW(info.window));
}

void Window::unfullscreen() {
  gtk_window_unfullscreen(GTK_WINDOW(info.window));
}

void Window::fullscreen(bool state) {
  (state == true) ? fullscreen() : unfullscreen();
}

bool Window::is_fullscreen() {
  return false;
};

void Window::set_text(const char *str) {
  gtk_window_set_title(GTK_WINDOW(info.window), str);
}

void Window::set_background_color(uint8 r, uint8 g, uint8 b) {
GdkColor color;
  color.pixel = (r << 16) | (g << 8) | (b);
  color.red   = (r << 8) | (r);
  color.green = (g << 8) | (g);
  color.blue  = (b << 8) | (b);
  gtk_widget_modify_bg(info.window, GTK_STATE_NORMAL, &color);
}

};
