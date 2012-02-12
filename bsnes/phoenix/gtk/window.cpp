static gint Window_close(GtkWidget *widget, GdkEvent *event, Window *window) {
  window->state.ignore = false;
  if(window->onClose) window->onClose();
  if(window->state.ignore == false) window->setVisible(false);
  return true;
}

static gboolean Window_expose(GtkWidget *widget, GdkEvent *event, Window *window) {
  if(window->state.backgroundColorOverride == false) return false;
  cairo_t *context = gdk_cairo_create(widget->window);

  Color color = window->backgroundColor();
  double red   = (double)color.red   / 255.0;
  double green = (double)color.green / 255.0;
  double blue  = (double)color.blue  / 255.0;
  double alpha = (double)color.alpha / 255.0;

  if(gdk_screen_is_composited(gdk_screen_get_default())) {
    cairo_set_source_rgba(context, red, green, blue, alpha);
  } else {
    cairo_set_source_rgb(context, red, green, blue);
  }

  cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
  cairo_paint(context);
  cairo_destroy(context);

  return false;
}

static gboolean Window_configure(GtkWidget *widget, GdkEvent *event, Window *window) {
  if(gtk_widget_get_realized(window->p.widget) == false) return false;
  GdkWindow *gdkWindow = gtk_widget_get_window(widget);

  GdkRectangle border, client;
  gdk_window_get_frame_extents(gdkWindow, &border);
  gdk_window_get_geometry(gdkWindow, 0, 0, &client.width, &client.height, 0);
  gdk_window_get_origin(gdkWindow, &client.x, &client.y);

  if(window->state.fullScreen == false) {
    //update geometry settings
    settings->frameGeometryX = client.x - border.x;
    settings->frameGeometryY = client.y - border.y;
    settings->frameGeometryWidth = border.width - client.width;
    settings->frameGeometryHeight = border.height - client.height;
    if(window->state.backgroundColorOverride == false) {
      GdkColor color = widget->style->bg[GTK_STATE_NORMAL];
      settings->windowBackgroundColor
      = ((uint8_t)(color.red   >> 8) << 16)
      + ((uint8_t)(color.green >> 8) <<  8)
      + ((uint8_t)(color.blue  >> 8) <<  0);
    }
    settings->save();
  }

  //move
  if(event->configure.x != window->p.lastConfigure.x
  || event->configure.y != window->p.lastConfigure.y
  ) {
    if(window->state.fullScreen == false) {
      window->state.geometry.x = client.x;
      window->state.geometry.y = client.y + window->p.menuHeight();
    }
    if(window->p.locked == false && window->onMove) window->onMove();
  }

  //size
  if(event->configure.width  != window->p.lastConfigure.width
  || event->configure.height != window->p.lastConfigure.height
  ) {
    if(window->state.fullScreen == false) {
      window->state.geometry.width  = client.width;
      window->state.geometry.height = client.height - window->p.menuHeight() - window->p.statusHeight();
    }

    for(auto &layout : window->state.layout) {
      Geometry geometry = window->geometry();
      geometry.x = geometry.y = 0;
      layout.setGeometry(geometry);
    }

    if(window->p.locked == false && window->onSize) window->onSize();
  }

  window->p.lastConfigure = event->configure;
  return false;
}

static gboolean Window_keyPressEvent(GtkWidget *widget, GdkEventKey *event, Window *window) {
  Keyboard::Keycode key = Keysym(event->keyval);
  if(key != Keyboard::Keycode::None && window->onKeyPress) window->onKeyPress(key);
  return false;
}

static gboolean Window_keyReleaseEvent(GtkWidget *widget, GdkEventKey *event, Window *window) {
  Keyboard::Keycode key = Keysym(event->keyval);
  if(key != Keyboard::Keycode::None && window->onKeyRelease) window->onKeyRelease(key);
  return false;
}

void pWindow::append(Layout &layout) {
  Geometry geometry = this->geometry();
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu &menu) {
  if(window.state.menuFont != "") menu.p.setFont(window.state.menuFont);
  else menu.p.setFont("Sans, 8");
  gtk_menu_shell_append(GTK_MENU_SHELL(this->menu), menu.p.widget);
  gtk_widget_show(menu.p.widget);
}

void pWindow::append(Widget &widget) {
  ((Sizable&)widget).state.window = &window;
  gtk_fixed_put(GTK_FIXED(formContainer), widget.p.gtkWidget, 0, 0);
  if(widget.state.font != "") widget.p.setFont(widget.state.font);
  else if(window.state.widgetFont != "") widget.p.setFont(window.state.widgetFont);
  else widget.p.setFont("Sans, 8");
  widget.setVisible(widget.visible());
}

Color pWindow::backgroundColor() {
  if(window.state.backgroundColorOverride) return window.state.backgroundColor;
  return {
    (uint8_t)(settings->windowBackgroundColor >> 16),
    (uint8_t)(settings->windowBackgroundColor >>  8),
    (uint8_t)(settings->windowBackgroundColor >>  0),
    255
  };
}

Geometry pWindow::frameMargin() {
  if(window.state.fullScreen) return { 0, menuHeight(), 0, menuHeight() + statusHeight() };
  return {
    settings->frameGeometryX,
    settings->frameGeometryY + menuHeight(),
    settings->frameGeometryWidth,
    settings->frameGeometryHeight + menuHeight() + statusHeight()
  };
}

bool pWindow::focused() {
  return gtk_window_is_active(GTK_WINDOW(widget));
}

Geometry pWindow::geometry() {
  if(window.state.fullScreen == true) {
    return { 0, menuHeight(), Desktop::size().width, Desktop::size().height - menuHeight() - statusHeight() };
  };
  return window.state.geometry;
}

void pWindow::remove(Layout &layout) {
}

void pWindow::remove(Menu &menu) {
  menu.p.orphan();
}

void pWindow::remove(Widget &widget) {
  widget.p.orphan();
}

void pWindow::setBackgroundColor(const Color &color) {
  GdkColor gdkColor;
  gdkColor.pixel = (color.red   << 16) | (color.green << 8) | (color.blue << 0);
  gdkColor.red   = (color.red   <<  8) | (color.red   << 0);
  gdkColor.green = (color.green <<  8) | (color.green << 0);
  gdkColor.blue  = (color.blue  <<  8) | (color.blue  << 0);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &gdkColor);
}

void pWindow::setFocused() {
  gtk_window_present(GTK_WINDOW(widget));
}

void pWindow::setFullScreen(bool fullScreen) {
  if(fullScreen == false) {
    gtk_window_unfullscreen(GTK_WINDOW(widget));
    gtk_window_set_resizable(GTK_WINDOW(widget), window.state.resizable);
    gtk_widget_set_size_request(widget, -1, -1);
    gdk_display_sync(gtk_widget_get_display(widget));
    setGeometry(window.state.geometry);
  } else {
    gtk_window_fullscreen(GTK_WINDOW(widget));
    gtk_widget_set_size_request(widget, Desktop::size().width, Desktop::size().height);
    gtk_window_set_resizable(GTK_WINDOW(widget), false);
  }
  gdk_display_sync(gtk_widget_get_display(widget));
}

void pWindow::setGeometry(const Geometry &geometry) {
  OS::processEvents();

  Geometry margin = frameMargin();
  gtk_window_move(GTK_WINDOW(widget), geometry.x - margin.x, geometry.y - margin.y);

//GdkGeometry geom;
//geom.min_width = 1;
//geom.min_height = 1;
//gtk_window_set_geometry_hints(GTK_WINDOW(widget), GTK_WIDGET(widget), &geom, GDK_HINT_MIN_SIZE);

  gtk_window_set_policy(GTK_WINDOW(widget), true, true, false);
  gtk_widget_set_size_request(formContainer, geometry.width, geometry.height);
  gtk_window_resize(GTK_WINDOW(widget), geometry.width, geometry.height + menuHeight() + statusHeight());

  for(auto &layout : window.state.layout) {
    Geometry geometry = this->geometry();
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }
}

void pWindow::setMenuFont(const string &font) {
  for(auto &item : window.state.menu) item.p.setFont(font);
}

void pWindow::setMenuVisible(bool visible) {
  gtk_widget_set_visible(menu, visible);
}

void pWindow::setResizable(bool resizable) {
  gtk_window_set_resizable(GTK_WINDOW(widget), resizable);
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(status), resizable);
}

void pWindow::setStatusFont(const string &font) {
  pFont::setFont(status, font);
}

void pWindow::setStatusText(const string &text) {
  gtk_statusbar_pop(GTK_STATUSBAR(status), 1);
  gtk_statusbar_push(GTK_STATUSBAR(status), 1, text);
}

void pWindow::setStatusVisible(bool visible) {
  gtk_widget_set_visible(status, visible);
}

void pWindow::setTitle(const string &text) {
  gtk_window_set_title(GTK_WINDOW(widget), text);
}

void pWindow::setVisible(bool visible) {
  gtk_widget_set_visible(widget, visible);
}

void pWindow::setWidgetFont(const string &font) {
  for(auto &item : window.state.widget) {
    if(item.state.font == "") item.setFont(font);
  }
}

void pWindow::constructor() {
  memset(&lastConfigure, 0, sizeof(GdkEventConfigure));
  widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  if(gdk_screen_is_composited(gdk_screen_get_default())) {
    gtk_widget_set_colormap(widget, gdk_screen_get_rgba_colormap(gdk_screen_get_default()));
  } else {
    gtk_widget_set_colormap(widget, gdk_screen_get_rgb_colormap(gdk_screen_get_default()));
  }

  gtk_window_set_resizable(GTK_WINDOW(widget), true);
  #if GTK_MAJOR_VERSION >= 3
  gtk_window_set_has_resize_grip(GTK_WINDOW(widget), false);
  #endif

  gtk_widget_set_app_paintable(widget, true);
  gtk_widget_add_events(widget, GDK_CONFIGURE);

  menuContainer = gtk_vbox_new(false, 0);
  gtk_container_add(GTK_CONTAINER(widget), menuContainer);
  gtk_widget_show(menuContainer);

  menu = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(menuContainer), menu, false, false, 0);

  formContainer = gtk_fixed_new();
  gtk_box_pack_start(GTK_BOX(menuContainer), formContainer, true, true, 0);
  gtk_widget_show(formContainer);

  statusContainer = gtk_event_box_new();
  status = gtk_statusbar_new();
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(status), true);
  gtk_container_add(GTK_CONTAINER(statusContainer), status);
  gtk_box_pack_start(GTK_BOX(menuContainer), statusContainer, false, false, 0);
  gtk_widget_show(statusContainer);

  setTitle("");
  setGeometry(window.state.geometry);
  setMenuFont("Sans, 8");
  setStatusFont("Sans, 8");

  g_signal_connect(G_OBJECT(widget), "delete-event", G_CALLBACK(Window_close), (gpointer)&window);
  g_signal_connect(G_OBJECT(widget), "expose-event", G_CALLBACK(Window_expose), (gpointer)&window);
  g_signal_connect(G_OBJECT(widget), "configure-event", G_CALLBACK(Window_configure), (gpointer)&window);
  g_signal_connect(G_OBJECT(widget), "key-press-event", G_CALLBACK(Window_keyPressEvent), (gpointer)&window);
  g_signal_connect(G_OBJECT(widget), "key-release-event", G_CALLBACK(Window_keyPressEvent), (gpointer)&window);
}

unsigned pWindow::menuHeight() {
  return window.state.menuVisible ? settings->menuGeometryHeight : 0;
}

unsigned pWindow::statusHeight() {
  return window.state.statusVisible ? settings->statusGeometryHeight : 0;
}
