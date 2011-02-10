static void Action_setFont(GtkWidget *widget, gpointer font);
static void Widget_setFont(GtkWidget *widget, gpointer font);

static gint Window_close(Window *window) {
  if(window->onClose) {
    if(window->onClose()) window->setVisible(false);
    return true;
  }
  window->setVisible(false);
  return true;
}

static void Window_configure(GtkWindow *widget, GdkEvent *event, Window *window) {
  if(gtk_widget_get_realized(window->object->widget) == false) return;
  window->updateFrameGeometry();

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
    if(window->window->layout) window->window->layout->setGeometry(geom);

    if(window->onSize) window->onSize();
  }
}

void Window::append(Menu &menu) {
  menu.action->font = window->defaultFont;
  if(window->defaultFont) Action_setFont(menu.object->widget, window->defaultFont->font->font);
  gtk_menu_bar_append(object->menu, menu.object->widget);
  gtk_widget_show(menu.object->widget);
}

void Window::setLayout(Layout &layout) {
  window->layout = &layout;
  layout.setParent(*this);
  Geometry geom = geometry();
  geom.x = geom.y = 0;
  layout.setGeometry(geom);
}

void Window::setResizable(bool resizable) {
  window->resizable = resizable;
  gtk_window_set_resizable(GTK_WINDOW(object->widget), resizable);
}

bool Window::focused() {
  return gtk_window_is_active(GTK_WINDOW(object->widget));
}

void Window::setFocused() {
  gtk_window_present(GTK_WINDOW(object->widget));
}

Geometry Window::frameGeometry() {
  return {
    window->x - settings.frameGeometryX, window->y - settings.frameGeometryY,
    window->width + settings.frameGeometryWidth, window->height + settings.frameGeometryHeight
  };
}

Geometry Window::geometry() {
  return { window->x, window->y, window->width, window->height };
}

void Window::setFrameGeometry(signed x, signed y, unsigned width, unsigned height) {
  setGeometry(
    x + settings.frameGeometryX, y + settings.frameGeometryY,
    width - settings.frameGeometryWidth, height - settings.frameGeometryHeight
  );
}

void Window::setGeometry(signed x, signed y, unsigned width, unsigned height) {
  window->x = x;
  window->y = y;
  window->width = width;
  window->height = height;

  gtk_window_move(GTK_WINDOW(object->widget), x - settings.frameGeometryX, y - settings.frameGeometryY);
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
  return window->fullscreen;
}

void Window::setFullscreen(bool fullscreen) {
  window->fullscreen = fullscreen;
  if(fullscreen == true) {
    gtk_window_fullscreen(GTK_WINDOW(object->widget));
    gtk_window_set_decorated(GTK_WINDOW(object->widget), false);
    gtk_widget_set_size_request(object->widget, gdk_screen_width(), gdk_screen_height());
    gtk_window_set_resizable(GTK_WINDOW(object->widget), window->resizable);
  } else {
    gtk_window_set_resizable(GTK_WINDOW(object->widget), true);
    gtk_widget_set_size_request(object->widget, -1, -1);
    gtk_window_set_decorated(GTK_WINDOW(object->widget), true);
    gtk_window_unfullscreen(GTK_WINDOW(object->widget));
  }
}

Window::Window() {
  window = new Window::Data;
  window->layout = 0;
  window->defaultFont = 0;
  window->resizable = true;
  window->fullscreen = false;
  window->x = 128;
  window->y = 128;
  window->width = 256;
  window->height = 256;

  object->widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_move(GTK_WINDOW(object->widget), window->x, window->y);

  gtk_window_set_resizable(GTK_WINDOW(object->widget), window->resizable);
  gtk_widget_set_app_paintable(object->widget, true);

  object->menuContainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(object->widget), object->menuContainer);
  gtk_widget_show(object->menuContainer);

  object->menu = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->menu, false, false, 0);

  object->formContainer = gtk_fixed_new();
  gtk_widget_set_size_request(object->formContainer, window->width, window->height);
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->formContainer, true, true, 0);
  gtk_widget_show(object->formContainer);

  object->statusContainer = gtk_event_box_new();
  object->status = gtk_statusbar_new();
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(object->status), false);
  gtk_container_add(GTK_CONTAINER(object->statusContainer), object->status);
  gtk_box_pack_start(GTK_BOX(object->menuContainer), object->statusContainer, false, false, 0);
  gtk_widget_show(object->statusContainer);

  g_signal_connect_swapped(G_OBJECT(object->widget), "delete_event", G_CALLBACK(Window_close), (gpointer)this);
  g_signal_connect(G_OBJECT(object->widget), "configure_event", G_CALLBACK(Window_configure), (gpointer)this);
}

//internal

void Window::updateFrameGeometry() {
  Display *display = XOpenDisplay(0);
  XWindowAttributes attributes, parentAttributes;
  XGetWindowAttributes(display, GDK_WINDOW_XID(object->widget->window), &attributes);
  X11Window rootWindow, parentWindow, *childWindow = 0;
  unsigned int childCount;
  XQueryTree(display, GDK_WINDOW_XID(object->widget->window), &rootWindow, &parentWindow, &childWindow, &childCount);
  XGetWindowAttributes(display, parentWindow, &parentAttributes);
  if(childWindow) XFree(childWindow);
  XCloseDisplay(display);

  settings.frameGeometryX = attributes.x;
  settings.frameGeometryY = attributes.y;
  settings.frameGeometryWidth = parentAttributes.width - attributes.width;
  settings.frameGeometryHeight = parentAttributes.height - attributes.height;
}
