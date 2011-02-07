static gint Window_close(Window *window) {
  if(window->onClose) {
    if(window->onClose()) window->setVisible(false);
    return true;
  }
  window->setVisible(false);
  return true;
}

static void Window_configure(GtkWindow *widget, GdkEvent *event, Window *window) {
  if(window->window->x != event->configure.x || window->window->y != event->configure.y) {
    window->window->x = event->configure.x;
    window->window->y = event->configure.y;
    if(window->onMove) window->onMove();
  }

  if(window->window->width != event->configure.width || window->window->height != event->configure.height) {
    window->window->width = event->configure.width;
    window->window->height = event->configure.height;
    Geometry geom = window->geometry();
    geom.x = geom.y = 0;
    if(window->window->layout) window->window->layout->update(geom);
    if(window->onResize) window->onResize();
  }
}

static gboolean Window_expose(GtkWidget *widget, GdkEventExpose *event, Window *window) {
}

void Window::create(unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  object->widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_move(GTK_WINDOW(object->widget), x, y);

  gtk_window_set_title(GTK_WINDOW(object->widget), text);
  gtk_window_set_resizable(GTK_WINDOW(object->widget), true);
  gtk_widget_set_app_paintable(object->widget, true);

  g_signal_connect_swapped(G_OBJECT(object->widget), "delete_event", G_CALLBACK(Window_close), (gpointer)this);
  g_signal_connect(G_OBJECT(object->widget), "configure_event", G_CALLBACK(Window_configure), (gpointer)this);
  g_signal_connect(G_OBJECT(object->widget), "expose_event", G_CALLBACK(Window_expose), (gpointer)this);

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

void Window::setLayout(Layout &layout) {
  window->layout = &layout;
  layout.setParent(*this);
  Geometry geom = geometry();
  geom.x = geom.y = 0;
  layout.update(geom);
}

bool Window::focused() {
  return gtk_window_is_active(GTK_WINDOW(object->widget));
}

void Window::setFocused() {
  gtk_window_present(GTK_WINDOW(object->widget));
}

Geometry Window::geometry() {
  return { window->x, window->y, window->width, window->height };
}

void Window::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  gtk_window_move(GTK_WINDOW(object->widget), window->x = x, window->y = y);
  gtk_widget_set_size_request(object->formContainer, window->width = width, window->height = height);
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

void Window::setVisible(bool visible) {
  gtk_widget_set_visible(object->widget, visible);
}

void Window::setMenuVisible(bool visible) {
  gtk_widget_set_visible(object->menu, visible);
}

void Window::setStatusVisible(bool visible) {
  gtk_widget_set_visible(object->status, visible);
}

bool Window::fullscreen() {
  return window->isFullscreen;
}

void Window::setFullscreen(bool fullscreen) {
  window->isFullscreen = fullscreen;
  if(fullscreen == true) {
    gtk_window_fullscreen(GTK_WINDOW(object->widget));
    gtk_window_set_decorated(GTK_WINDOW(object->widget), false);
    gtk_widget_set_size_request(object->widget, gdk_screen_width(), gdk_screen_height());
  } else {
    gtk_widget_set_size_request(object->widget, -1, -1);
    gtk_window_set_decorated(GTK_WINDOW(object->widget), true);
    gtk_window_unfullscreen(GTK_WINDOW(object->widget));

    //at this point, GTK+ has not updated window geometry
    //this causes Window::geometry() calls to return incorrect info
    //thus, wait until the geometry has changed before continuing
    Geometry geom;
    time_t startTime = time(0);
    do {
      OS::run();
      Geometry geom = geometry();
      if(startTime - time(0) > 3) break;  //prevent application from freezing
    } while(geom.x == 0 && geom.y == 0 && geom.width == gdk_screen_width() && geom.height == gdk_screen_height());
  }
}

Window::Window() {
  window = new Window::Data;
  window->layout = 0;
  window->defaultFont = 0;
  window->isFullscreen = false;
  window->x = 0;
  window->y = 0;
  window->width = 0;
  window->height = 0;
}
