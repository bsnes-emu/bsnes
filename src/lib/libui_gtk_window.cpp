namespace libui {

gint libui_window_close(GtkWidget *w, GdkEventAny *any, Window *window) {
  if(window) { return !window->close(); }
  return FALSE; //destroy window by default
}

gint libui_window_keydown(GtkWidget *w, GdkEventKey *key, Window *window) {
  if(window) { window->keydown(key->keyval); }
  return FALSE;
}

gint libui_window_keyup(GtkWidget *w, GdkEventKey *key, Window *window) {
  if(window) { window->keyup(key->keyval); }
  return FALSE;
}

void libui_control_clicked(Control *control) {
  if(control && control->owner) { control->owner->clicked(control); }
}

WindowHandle Window::handle() {
  return (WindowHandle)(GDK_WINDOW_XWINDOW(info.window->window));
}

void Window::attach(Control &control, uint x, uint y, bool attach_to_window) {
  info.control[info.control_index] = &control;
  control.id    = info.control_index++;
  control.owner = this;
  if(attach_to_window == true) {
    gtk_fixed_put(GTK_FIXED(info.container), control.widget, x, y);
  }

  switch(control.type) {

  case Control::Button:
  case Control::Checkbox:
  case Control::Radiobox: {
    g_signal_connect_swapped(G_OBJECT(control.widget), "clicked",
      G_CALLBACK(libui_control_clicked), (gpointer)&control);
  } break;

  }
}

void Window::create(const char *style, uint width, uint height, const char *caption) {
  info.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(info.window), caption ? caption : "");
  gtk_window_set_resizable(GTK_WINDOW(info.window), false);
  g_signal_connect(G_OBJECT(info.window), "delete_event",      G_CALLBACK(libui_window_close),   (gpointer)this);
  g_signal_connect(G_OBJECT(info.window), "key_press_event",   G_CALLBACK(libui_window_keydown), (gpointer)this);
  g_signal_connect(G_OBJECT(info.window), "key_release_event", G_CALLBACK(libui_window_keyup),   (gpointer)this);

stringarray part;
  split(part, "|", style);
  for(int i = 0; i < count(part); i++) {
    if(part[i] == "menu") { info.has_menu = true; }
    if(part[i] == "center") { gtk_window_set_position(GTK_WINDOW(info.window), GTK_WIN_POS_CENTER_ALWAYS); }
  }

  info.vcontainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(info.window), info.vcontainer);
  gtk_widget_show(info.vcontainer);

  if(info.has_menu == true) {
  MenuGroup *group = new MenuGroup();
    info.control[info.control_index] = group;

    group->id     = info.control_index++;
    group->owner  = this;
    group->type   = Control::MenuGroup;
    group->master = true;
    group->widget = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(info.vcontainer), group->widget, false, false, 0);
    gtk_widget_show(group->widget);

    info.menu_group[++info.menu_group_index] = group;
  }

  info.container = gtk_fixed_new();
  gtk_widget_set_size_request(info.container, width, height);
  gtk_box_pack_end(GTK_BOX(info.vcontainer), info.container, true, true, 0);
  gtk_widget_show(info.container);
}

void Window::focus() {
  gtk_window_present(GTK_WINDOW(info.window));
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

void Window::set_background_color(uint8 r, uint8 g, uint8 b) {
GdkColor color;
  color.pixel = (r << 16) | (g << 8) | (b);
  color.red   = (r << 8) | (r);
  color.green = (g << 8) | (g);
  color.blue  = (b << 8) | (b);
  gtk_widget_modify_bg(info.window, GTK_STATE_NORMAL, &color);
}

};
