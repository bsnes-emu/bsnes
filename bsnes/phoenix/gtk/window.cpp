static gint Window_close(Window *window) {
  if(window->onClose) {
    if(window->onClose()) window->setVisible(false);
    return true;
  }
  window->setVisible(false);
  return true;
}

void Window::create(unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  object->widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_move(GTK_WINDOW(object->widget), x, y);

  gtk_window_set_title(GTK_WINDOW(object->widget), text);
  gtk_window_set_resizable(GTK_WINDOW(object->widget), false);
  gtk_widget_set_app_paintable(object->widget, true);

  g_signal_connect_swapped(G_OBJECT(object->widget), "delete_event", G_CALLBACK(Window_close), (gpointer)this);

  object->menuContainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(object->widget), object->menuContainer);
  gtk_widget_show(object->menuContainer);

  object->menu = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->menu, false, false, 0);

  object->formContainer = gtk_fixed_new();
  gtk_widget_set_size_request(object->formContainer, width, height);
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->formContainer, true, true, 0);
  gtk_widget_show(object->formContainer);

  object->statusContainer = gtk_event_box_new();
  object->status = gtk_statusbar_new();
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(object->status), false);
  gtk_container_add(GTK_CONTAINER(object->statusContainer), object->status);
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->statusContainer, false, false, 0);
  gtk_widget_show(object->statusContainer);

  gtk_widget_realize(object->widget);
}

bool Window::focused() {
  return gtk_window_is_active(GTK_WINDOW(object->widget));
}

void Window::setFocused() {
  gtk_window_present(GTK_WINDOW(object->widget));
}

Geometry Window::geometry() {
  gint x, y, width, height;
  gtk_window_get_position(GTK_WINDOW(object->widget), &x, &y);
  gtk_widget_get_size_request(object->formContainer, &width, &height);
  return Geometry(x, y, width, height);
}

void Window::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  gtk_window_move(GTK_WINDOW(object->widget), x, y);
  gtk_widget_set_size_request(object->formContainer, width, height);
}

void Window::setDefaultFont(Font &font) {
  window->defaultFont = &font;
}

void Window::setFont(Font &font) {
  Widget_setFont(object->status, font.font->font);
}

void Window::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
  GdkColor color;
  color.pixel = (red << 16) | (green << 8) | (blue << 0);
  color.red = (red << 8) | (red << 0);
  color.green = (green << 8) | (green << 0);
  color.blue = (blue << 8) | (blue << 0);
  gtk_widget_modify_bg(object->widget, GTK_STATE_NORMAL, &color);
}

void Window::setTitle(const string &text) {
  gtk_window_set_title(GTK_WINDOW(object->widget), text);
}

void Window::setStatusText(const string &text) {
  gtk_statusbar_pop(GTK_STATUSBAR(object->status), 1);
  gtk_statusbar_push(GTK_STATUSBAR(object->status), 1, text);
}

void Window::setMenuVisible(bool visible) {
  gtk_widget_set_visible(object->menu, visible);
}

void Window::setStatusVisible(bool visible) {
  gtk_widget_set_visible(object->status, visible);
}

Window::Window() {
  window = new Window::Data;
  window->defaultFont = 0;
}
