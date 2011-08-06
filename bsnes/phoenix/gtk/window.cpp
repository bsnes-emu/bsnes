static void Action_setFont(GtkWidget *widget, gpointer font);
static void Widget_setFont(GtkWidget *widget, gpointer font);

static gint Window_close(GtkWidget *widget, GdkEvent *event, Window *window) {
  if(window->onClose) window->onClose();
  window->setVisible(false);
  return true;
}

static gboolean Window_configure(GtkWidget *widget, GdkEvent *event, Window *window) {
  if(gtk_widget_get_realized(window->p.widget) == false) return false;
  GdkWindow *gdkWindow = gtk_widget_get_window(widget);

  //update geometry settings
  GdkRectangle border, client;
  gdk_window_get_frame_extents(gdkWindow, &border);
  gdk_window_get_geometry(gdkWindow, 0, 0, &client.width, &client.height, 0);
  gdk_window_get_origin(gdkWindow, &client.x, &client.y);

  settings.frameGeometryX = client.x - border.x;
  settings.frameGeometryY = client.y - border.y;
  settings.frameGeometryWidth = border.width - client.width;
  settings.frameGeometryHeight = border.height - client.height;

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

    foreach(layout, window->state.layout) {
      Geometry geometry = window->geometry();
      geometry.x = geometry.y = 0;
      layout.setGeometry(geometry);
    }

    if(window->p.locked == false && window->onSize) window->onSize();
  }

  window->p.lastConfigure = event->configure;
  return false;
}

void pWindow::append(Layout &layout) {
  layout.setParent(window);
  Geometry geometry = this->geometry();
  geometry.x = geometry.y = 0;
  layout.setGeometry(geometry);
}

void pWindow::append(Menu &subMenu) {
  if(window.state.menuFont) subMenu.p.setFont(*window.state.menuFont);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), subMenu.p.widget);
  gtk_widget_show(subMenu.p.widget);
}

void pWindow::append(Widget &widget) {
  widget.p.parentWindow = this;
  if(!widget.state.font && window.state.widgetFont) {
    widget.setFont(*window.state.widgetFont);
  }
  gtk_fixed_put(GTK_FIXED(formContainer), widget.p.gtkWidget, 0, 0);
  widget.setVisible();
}

Color pWindow::backgroundColor() {
  if(window.state.backgroundColorOverride) return window.state.backgroundColor;
  GdkColor color = widget->style->bg[GTK_STATE_NORMAL];
  return { (uint8_t)(color.red >> 8), (uint8_t)(color.green >> 8), (uint8_t)(color.blue >> 8), 255 };
}

Geometry pWindow::frameMargin() {
  if(window.state.fullScreen) return { 0, menuHeight(), 0, menuHeight() + statusHeight() };
  return {
    settings.frameGeometryX,
    settings.frameGeometryY + menuHeight(),
    settings.frameGeometryWidth,
    settings.frameGeometryHeight + menuHeight() + statusHeight()
  };
}

bool pWindow::focused() {
  return gtk_window_is_active(GTK_WINDOW(widget));
}

Geometry pWindow::geometry() {
  if(window.state.fullScreen == true) {
    return { 0, menuHeight(), OS::desktopGeometry().width, OS::desktopGeometry().height - menuHeight() - statusHeight() };
  };
  return window.state.geometry;
}

void pWindow::setBackgroundColor(const Color &color) {
  GdkColor gdkColor;
  gdkColor.pixel = (color.red << 16) | (color.green << 8) | (color.blue << 0);
  gdkColor.red = (color.red << 8) | (color.red << 0);
  gdkColor.green = (color.green << 8) | (color.green << 0);
  gdkColor.blue = (color.blue << 8) | (color.blue << 0);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &gdkColor);
}

void pWindow::setFocused() {
  gtk_window_present(GTK_WINDOW(widget));
}

void pWindow::setFullScreen(bool fullScreen) {
  if(fullScreen == false) {
    gtk_window_unfullscreen(GTK_WINDOW(widget));
    gtk_window_set_resizable(GTK_WINDOW(widget), window.state.resizable);
    gtk_window_set_decorated(GTK_WINDOW(widget), true);
    for(unsigned n = 0; n < 4; n++) {
      setGeometry(window.state.geometry);
      gtk_widget_set_size_request(widget, -1, -1);
      OS::processEvents();
      usleep(2000);
    }
  } else {
    gtk_window_fullscreen(GTK_WINDOW(widget));
    gtk_window_set_decorated(GTK_WINDOW(widget), false);
    gtk_widget_set_size_request(widget, OS::desktopGeometry().width, OS::desktopGeometry().height);
    gtk_window_set_resizable(GTK_WINDOW(widget), false);
  }
}

void pWindow::setGeometry(const Geometry &geometry) {
  Geometry margin = frameMargin();
  gtk_window_move(GTK_WINDOW(widget), geometry.x - margin.x, geometry.y - margin.y);
  gtk_window_resize(GTK_WINDOW(widget), 1, 1);
  gtk_widget_set_size_request(formContainer, geometry.width, geometry.height);
  foreach(layout, window.state.layout) {
    Geometry geometry = this->geometry();
    geometry.x = geometry.y = 0;
    layout.setGeometry(geometry);
  }
}

void pWindow::setMenuFont(Font &font) {
  foreach(item, window.state.menu) item.p.setFont(font);
}

void pWindow::setMenuVisible(bool visible) {
  gtk_widget_set_visible(menu, visible);
}

void pWindow::setResizable(bool resizable) {
  gtk_window_set_resizable(GTK_WINDOW(widget), resizable);
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(status), resizable);
}

void pWindow::setStatusFont(Font &font) {
  Widget_setFont(status, (gpointer)font.p.gtkFont);
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

void pWindow::setWidgetFont(Font &font) {
  foreach(item, window.state.widget) {
    if(!item.state.font) item.setFont(font);
  }
}

void pWindow::constructor() {
  widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(widget), true);
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

  g_signal_connect(G_OBJECT(widget), "delete-event", G_CALLBACK(Window_close), (gpointer)&window);
  g_signal_connect(G_OBJECT(widget), "configure-event", G_CALLBACK(Window_configure), (gpointer)&window);
}

unsigned pWindow::menuHeight() {
  return window.state.menuVisible ? settings.menuGeometryHeight : 0;
}

unsigned pWindow::statusHeight() {
  return window.state.statusVisible ? settings.statusGeometryHeight : 0;
}
