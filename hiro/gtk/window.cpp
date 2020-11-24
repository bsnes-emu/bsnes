#if defined(Hiro_Window)

namespace hiro {

static auto Window_close(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  if(p->state().onClose) {
    p->self().doClose();
  } else {
    p->self().setVisible(false);
  }
  if(p->self().modal() && !p->self().visible()) p->self().setModal(false);
  return true;
}

//GTK3 draw: called into by GTK2 expose-event
static auto Window_draw(GtkWidget* widget, cairo_t* context, pWindow* p) -> signed {
  if(auto color = p->state().backgroundColor) {
    double red   = (double)color.red()   / 255.0;
    double green = (double)color.green() / 255.0;
    double blue  = (double)color.blue()  / 255.0;
    double alpha = (double)color.alpha() / 255.0;

    if(gdk_screen_is_composited(gdk_screen_get_default())
    && gdk_screen_get_rgba_visual(gdk_screen_get_default())
    ) {
      cairo_set_source_rgba(context, red, green, blue, alpha);
    } else {
      cairo_set_source_rgb(context, red, green, blue);
    }

    cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
    cairo_paint(context);
  } else {
    #if HIRO_GTK==3
    auto style = gtk_widget_get_style_context(widget);
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    gtk_render_background(style, context, 0, 0, allocation.width, allocation.height);
    #endif
  }

  return false;
}

//GTK2 expose-event
static auto Window_expose(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  cairo_t* context = gdk_cairo_create(gtk_widget_get_window(widget));
  Window_draw(widget, context, p);
  cairo_destroy(context);
  return false;
}

static auto Window_configure(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  p->_synchronizeMargin();
  return false;
}

static auto Window_drop(GtkWidget* widget, GdkDragContext* context, signed x, signed y,
GtkSelectionData* data, unsigned type, unsigned timestamp, pWindow* p) -> void {
  if(!p->state().droppable) return;
  auto paths = DropPaths(data);
  if(!paths) return;
  p->self().doDrop(paths);
}

static auto Window_getPreferredWidth(GtkWidget* widget, int* minimalWidth, int* naturalWidth) -> void {
  if(auto p = (pWindow*)g_object_get_data(G_OBJECT(widget), "hiro::window")) {
    *minimalWidth = 1;
    *naturalWidth = p->state().geometry.width();
  }
}

static auto Window_getPreferredHeight(GtkWidget* widget, int* minimalHeight, int* naturalHeight) -> void {
  if(auto p = (pWindow*)g_object_get_data(G_OBJECT(widget), "hiro::window")) {
    *minimalHeight = 1;
    *naturalHeight = p->state().geometry.height();
  }
}

static auto Window_keyPress(GtkWidget* widget, GdkEventKey* event, pWindow* p) -> signed {
  if(auto key = pKeyboard::_translate(event->keyval)) {
    p->self().doKeyPress(key);
  }
  if(p->state().dismissable && event->keyval == GDK_KEY_Escape) {
    if(p->state().onClose) {
      p->self().doClose();
    } else {
      p->self().setVisible(false);
    }
    if(p->state().modal && !p->pObject::state().visible) p->self().setModal(false);
  }
  return false;
}

static auto Window_keyRelease(GtkWidget* widget, GdkEventKey* event, pWindow* p) -> signed {
  if(auto key = pKeyboard::_translate(event->keyval)) {
    p->self().doKeyRelease(key);
  }
  return false;
}

static auto Window_realize(GtkWidget* widget, pWindow* p) -> void {
}

static auto Window_sizeAllocate(GtkWidget* widget, GtkAllocation* allocation, pWindow* p) -> void {
  p->_synchronizeState();
  p->_synchronizeGeometry();
  p->_synchronizeMargin();
  return;
}

static auto Window_sizeRequest(GtkWidget* widget, GtkRequisition* requisition, pWindow* p) -> void {
  requisition->width  = p->state().geometry.width();
  requisition->height = p->state().geometry.height();
}

static auto Window_stateEvent(GtkWidget* widget, GdkEvent* event, pWindow* p) -> void {
  p->_synchronizeState();

  if(event->type == GDK_WINDOW_STATE) {
    auto windowStateEvent = (GdkEventWindowState*)event;
    if(windowStateEvent->changed_mask & GDK_WINDOW_STATE_MAXIMIZED) {
      p->state().maximized = windowStateEvent->new_window_state & GDK_WINDOW_STATE_MAXIMIZED;
    }
    if(windowStateEvent->changed_mask & GDK_WINDOW_STATE_ICONIFIED) {
      p->state().minimized = windowStateEvent->new_window_state & GDK_WINDOW_STATE_ICONIFIED;
    }
  }
}

static auto Window_unrealize(GtkWidget* widget, pWindow* p) -> void {
}

auto pWindow::construct() -> void {
  widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(widget), true);

  //if program was given a name, try and set the window taskbar icon from one of the pixmaps folders
  if(!Application::state().name);
  else if(_setIcon({Path::user(), ".local/share/icons/"}));
  else if(_setIcon("/usr/local/share/pixmaps/"));
  else if(_setIcon("/usr/share/pixmaps/"));

  auto visual = gdk_screen_get_rgba_visual(gdk_screen_get_default());
  if(!visual) visual = gdk_screen_get_system_visual(gdk_screen_get_default());
  if(visual) gtk_widget_set_visual(widget, visual);

  gtk_widget_set_app_paintable(widget, true);
  gtk_widget_add_events(widget, GDK_CONFIGURE);

  #if HIRO_GTK==2
  menuContainer = gtk_vbox_new(false, 0);
  #elif HIRO_GTK==3
  menuContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  #endif
  gtk_container_add(GTK_CONTAINER(widget), menuContainer);
  gtk_widget_show(menuContainer);

  gtkMenu = gtk_menu_bar_new();
  gtk_box_pack_start(GTK_BOX(menuContainer), gtkMenu, false, false, 0);
  _setMenuColor();

  formContainer = gtk_fixed_new();
  gtk_box_pack_start(GTK_BOX(menuContainer), formContainer, true, true, 0);
  gtk_widget_show(formContainer);

  statusContainer = gtk_event_box_new();
  gtkStatus = gtk_statusbar_new();
  #if HIRO_GTK==2
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(gtkStatus), true);
  #elif HIRO_GTK==3
  gtk_window_set_has_resize_grip(GTK_WINDOW(widget), true);
  #endif
  gtk_container_add(GTK_CONTAINER(statusContainer), gtkStatus);
  gtk_box_pack_start(GTK_BOX(menuContainer), statusContainer, false, false, 0);
  gtk_widget_show(statusContainer);

  setBackgroundColor(state().backgroundColor);
  setDroppable(state().droppable);
  setGeometry(state().geometry);
  setResizable(state().resizable);
  setMaximized(state().maximized);
  setMinimized(state().minimized);
  setTitle(state().title);

  g_signal_connect(G_OBJECT(widget), "delete-event", G_CALLBACK(Window_close), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(widget), "expose-event", G_CALLBACK(Window_expose), (gpointer)this);
  #elif HIRO_GTK==3
  g_signal_connect(G_OBJECT(widget), "draw", G_CALLBACK(Window_draw), (gpointer)this);
  #endif
  g_signal_connect(G_OBJECT(widget), "configure-event", G_CALLBACK(Window_configure), (gpointer)this);
  g_signal_connect(G_OBJECT(widget), "drag-data-received", G_CALLBACK(Window_drop), (gpointer)this);
  g_signal_connect(G_OBJECT(widget), "key-press-event", G_CALLBACK(Window_keyPress), (gpointer)this);
  g_signal_connect(G_OBJECT(widget), "key-release-event", G_CALLBACK(Window_keyRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(widget), "realize", G_CALLBACK(Window_realize), (gpointer)this);
  g_signal_connect(G_OBJECT(formContainer), "size-allocate", G_CALLBACK(Window_sizeAllocate), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(formContainer), "size-request", G_CALLBACK(Window_sizeRequest), (gpointer)this);
  #elif HIRO_GTK==3
  auto widgetClass = GTK_WIDGET_GET_CLASS(formContainer);
  widgetClass->get_preferred_width  = Window_getPreferredWidth;
  widgetClass->get_preferred_height = Window_getPreferredHeight;
  #endif
  g_signal_connect(G_OBJECT(widget), "unrealize", G_CALLBACK(Window_unrealize), (gpointer)this);
  g_signal_connect(G_OBJECT(widget), "window-state-event", G_CALLBACK(Window_stateEvent), (gpointer)this);

  g_object_set_data(G_OBJECT(widget), "hiro::window", (gpointer)this);
  g_object_set_data(G_OBJECT(formContainer), "hiro::window", (gpointer)this);

  pApplication::state().windows.append(this);
}

auto pWindow::destruct() -> void {
  for(uint offset : range(pApplication::state().windows.size())) {
    if(pApplication::state().windows[offset] == this) {
      pApplication::state().windows.remove(offset);
      break;
    }
  }

  gtk_widget_destroy(widget);
}

auto pWindow::append(sMenuBar menuBar) -> void {
  _setMenuEnabled(menuBar->enabled(true));
  _setMenuFont(menuBar->font(true));
  _setMenuVisible(menuBar->visible(true));
}

auto pWindow::append(sSizable sizable) -> void {
}

auto pWindow::append(sStatusBar statusBar) -> void {
  _setStatusEnabled(statusBar->enabled(true));
  _setStatusFont(statusBar->font(true));
  _setStatusText(statusBar->text());
  _setStatusVisible(statusBar->visible(true));
}

auto pWindow::focused() const -> bool {
  return gtk_window_is_active(GTK_WINDOW(widget));
}

auto pWindow::frameMargin() const -> Geometry {
  if(state().fullScreen) return {
    0, _menuHeight(),
    0, _menuHeight() + _statusHeight()
  };

  return {
    settings.geometry.frameX,
    settings.geometry.frameY + _menuHeight(),
    settings.geometry.frameWidth,
    settings.geometry.frameHeight + _menuHeight() + _statusHeight()
  };
}

auto pWindow::handle() const -> uintptr_t {
  #if defined(DISPLAY_WINDOWS)
  return (uintptr)GDK_WINDOW_HWND(gtk_widget_get_window(widget));
  #endif

  #if defined(DISPLAY_XORG)
  return GDK_WINDOW_XID(gtk_widget_get_window(widget));
  #endif

  return (uintptr)nullptr;
}

auto pWindow::monitor() const -> uint {
  if(!gtk_widget_get_realized(widget)) return 0;
  auto window = gtk_widget_get_window(widget);
  return gdk_screen_get_monitor_at_window(gdk_screen_get_default(), window);
}

auto pWindow::remove(sMenuBar menuBar) -> void {
  _setMenuVisible(false);
}

auto pWindow::remove(sSizable sizable) -> void {
}

auto pWindow::remove(sStatusBar statusBar) -> void {
  _setStatusVisible(false);
}

auto pWindow::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
  _setMenuColor();
}

auto pWindow::setDismissable(bool dismissable) -> void {
}

auto pWindow::setDroppable(bool droppable) -> void {
  gtk_drag_dest_set(widget, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
  if(droppable) gtk_drag_dest_add_uri_targets(widget);
}

auto pWindow::setEnabled(bool enabled) -> void {
  if(auto& menuBar = state().menuBar) {
    if(auto self = menuBar->self()) self->setEnabled(menuBar->enabled(true));
  }

  if(auto& statusBar = state().statusBar) {
    if(auto self = statusBar->self()) self->setEnabled(statusBar->enabled(true));
  }
}

auto pWindow::setFocused() -> void {
  gtk_window_present(GTK_WINDOW(widget));
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  if(fullScreen) {
    gtk_window_fullscreen(GTK_WINDOW(widget));
  } else {
    gtk_window_unfullscreen(GTK_WINDOW(widget));
  }

  auto time = chrono::millisecond();
  while(chrono::millisecond() - time < 20) {
    Application::processEvents();
  }
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  auto margin = frameMargin();

  setMaximumSize(state().maximumSize);
  setMinimumSize(state().minimumSize);

  auto time1 = chrono::millisecond();
  while(chrono::millisecond() - time1 < 20) {
    Application::processEvents();
  }

  gtk_window_move(GTK_WINDOW(widget), geometry.x() - margin.x(), geometry.y() - margin.y());
  gtk_window_resize(GTK_WINDOW(widget), geometry.width(), geometry.height() + _menuHeight() + _statusHeight());

  auto time2 = chrono::millisecond();
  while(chrono::millisecond() - time2 < 20) {
    Application::processEvents();
  }
}

auto pWindow::setMaximized(bool maximized) -> void {
  auto lock = acquire();
  if(maximized) {
    gtk_window_maximize(GTK_WINDOW(widget));
  } else {
    gtk_window_unmaximize(GTK_WINDOW(widget));
  }
}

auto pWindow::setMaximumSize(Size size) -> void {
  if(!state().resizable) size = state().geometry.size();

  //TODO: this doesn't have any effect in GTK2 or GTK3
  GdkGeometry geometry;
  if(size.height()) size.setHeight(size.height() + _menuHeight() + _statusHeight());
  geometry.max_width  = !state().resizable ? state().geometry.width()  : size.width()  ? size.width()  : 32767;
  geometry.max_height = !state().resizable ? state().geometry.height() : size.height() ? size.height() : 32767;
  gtk_window_set_geometry_hints(GTK_WINDOW(widget), nullptr, &geometry, GDK_HINT_MAX_SIZE);
}

auto pWindow::setMinimized(bool minimized) -> void {
  auto lock = acquire();
  if(minimized) {
    gtk_window_iconify(GTK_WINDOW(widget));
  } else {
    gtk_window_deiconify(GTK_WINDOW(widget));
  }
}

auto pWindow::setMinimumSize(Size size) -> void {
  if(!state().resizable) size = state().geometry.size();

  //for GTK3
  gtk_widget_set_size_request(formContainer, size.width(), size.height());

  //for GTK2
  GdkGeometry geometry;
  if(size.height()) size.setHeight(size.height() + _menuHeight() + _statusHeight());
  geometry.min_width  = !state().resizable ? state().geometry.width()  : size.width()  ? size.width()  : 1;
  geometry.min_height = !state().resizable ? state().geometry.height() : size.height() ? size.height() : 1;
  gtk_window_set_geometry_hints(GTK_WINDOW(widget), nullptr, &geometry, GDK_HINT_MIN_SIZE);  //for GTK2
}

auto pWindow::setModal(bool modal) -> void {
  if(modal) {
    gtk_window_set_modal(GTK_WINDOW(widget), true);
    while(!Application::state().quit && state().modal) {
      if(Application::state().onMain) {
        Application::doMain();
      } else {
        usleep(20 * 1000);
      }
      Application::processEvents();
    }
    gtk_window_set_modal(GTK_WINDOW(widget), false);
  }
}

auto pWindow::setResizable(bool resizable) -> void {
  gtk_window_set_resizable(GTK_WINDOW(widget), resizable);
  #if HIRO_GTK==2
  gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(gtkStatus), resizable);
  #elif HIRO_GTK==3
  bool statusBarVisible = false;
  if(auto statusBar = state().statusBar) statusBarVisible = statusBar->visible();
  gtk_window_set_has_resize_grip(GTK_WINDOW(widget), resizable && statusBarVisible);
  #endif

  setMaximumSize(state().maximumSize);
  setMinimumSize(state().minimumSize);
}

auto pWindow::setTitle(const string& title) -> void {
  gtk_window_set_title(GTK_WINDOW(widget), title ? title : " ");
}

auto pWindow::setVisible(bool visible) -> void {
  gtk_widget_set_visible(widget, visible);
  _synchronizeGeometry();
  _synchronizeMargin();
}

auto pWindow::_append(mWidget& widget) -> void {
  if(auto pWidget = widget.self()) {
    if(auto parent = widget.parentWidget(true)) {
      if(auto instance = parent->self()) {
        pWidget->gtkParent = instance->container(widget);
      }
    } else {
      pWidget->gtkParent = formContainer;
    }
    if(pWidget->gtkParent) {
      gtk_fixed_put(GTK_FIXED(pWidget->gtkParent), pWidget->gtkWidget, 0, 0);
    }
  }
}

auto pWindow::_append(mMenu& menu) -> void {
  if(auto pMenu = menu.self()) {
    gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), pMenu->widget);
  }
}

auto pWindow::_menuHeight() const -> int {
  if(auto& menuBar = state().menuBar) {
    if(menuBar->visible()) {
      return settings.geometry.menuHeight + _menuTextHeight();
    }
  }
  return 0;
}

auto pWindow::_menuTextHeight() const -> int {
  int height = 0;
  if(auto& menuBar = state().menuBar) {
    for(auto& menu : menuBar->state.menus) {
      height = max(height, menu->font(true).size(menu->text()).height());
    }
  }
  return height;
}

auto pWindow::_setIcon(const string& pathname) -> bool {
  string filename;

  filename = {pathname, Application::state().name, ".svg"};
  if(file::exists(filename)) {
    gtk_window_set_icon_from_file(GTK_WINDOW(widget), filename, nullptr);
    return true;
  }

  filename = {pathname, Application::state().name, ".png"};
  if(file::exists(filename)) {
    //maximum image size GTK+ supports is 256x256; scale image down if necessary to prevent error
    image icon(filename);
    icon.scale(min(256u, icon.width()), min(256u, icon.height()), true);
    auto pixbuf = CreatePixbuf(icon);
    gtk_window_set_icon(GTK_WINDOW(widget), pixbuf);
    g_object_unref(G_OBJECT(pixbuf));
    return true;
  }

  return false;
}

//fix for menu-bar colors with custom window background colors (only needed for GTK3)
auto pWindow::_setMenuColor() -> void {
  #if HIRO_GTK==3
  //if no window background color was set, we can use the original menu-bar color
  if(!state().backgroundColor) {
    gtk_widget_override_color(gtkMenu, GTK_STATE_FLAG_NORMAL, nullptr);
    gtk_widget_override_background_color(gtkMenu, GTK_STATE_FLAG_NORMAL, nullptr);
    return;
  }

  //if Window::setBackgroundColor is used, the menu-bar will be the wrong color
  //to prevent this from being an issue, we retrieve the theme background color and set it ourselves here
  GtkStyleContext* context = gtk_widget_get_style_context(gtkMenu);
  GdkRGBA gdkColor;
  gtk_style_context_lookup_color(context, "theme_bg_color", &gdkColor);
  if(gdkColor.alpha == 0.0) {
    //if this fails (it should not happen); set the menubar to black text on a light gray background
    gdk_rgba_parse(&gdkColor, "#000000");
    gtk_widget_override_color(gtkMenu, GTK_STATE_FLAG_NORMAL, &gdkColor);
    gdk_rgba_parse(&gdkColor, "#e8e8e8");
  }
  gtk_widget_override_background_color(gtkMenu, GTK_STATE_FLAG_NORMAL, &gdkColor);
  #endif
}

auto pWindow::_setMenuEnabled(bool enabled) -> void {
  gtk_widget_set_sensitive(gtkMenu, enabled);
}

auto pWindow::_setMenuFont(const Font& font) -> void {
  pFont::setFont(gtkMenu, font);
}

auto pWindow::_setMenuVisible(bool visible) -> void {
  gtk_widget_set_visible(gtkMenu, visible);
}

auto pWindow::_setStatusEnabled(bool enabled) -> void {
  gtk_widget_set_sensitive(gtkStatus, enabled);
}

auto pWindow::_setStatusFont(const Font& font) -> void {
  pFont::setFont(gtkStatus, font);
}

auto pWindow::_setStatusText(const string& text) -> void {
  gtk_statusbar_pop(GTK_STATUSBAR(gtkStatus), 1);
  gtk_statusbar_push(GTK_STATUSBAR(gtkStatus), 1, text);
}

auto pWindow::_setStatusVisible(bool visible) -> void {
  gtk_widget_set_visible(gtkStatus, visible);
  setResizable(self().resizable());
}

auto pWindow::_statusHeight() const -> int {
  if(auto& statusBar = state().statusBar) {
    if(statusBar->visible()) {
      return settings.geometry.statusHeight + _statusTextHeight();
    }
  }
  return 0;
}

auto pWindow::_statusTextHeight() const -> int {
  int height = 0;
  if(auto& statusBar = state().statusBar) {
    height = statusBar->font(true).size(statusBar->text()).height();
  }
  return height;
}

//GTK is absolutely hopeless with window sizing
//it will send size-allocate events during resizing of widgets during size-allocate events
//instead of fighting with configure-event and size-allocate, just poll the state instead
auto pWindow::_synchronizeGeometry() -> void {
  if(locked()) return;
  auto lock = acquire();

  if(!gtk_widget_get_realized(widget)) return;
  if(!gtk_widget_get_visible(widget)) return;

  //get_allocation(formContainer) returns the same values as get_allocation(widget) ...
  //as a result, we have to compensate for the window margin ourselves here.
  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  allocation.height -= _menuHeight();
  allocation.height -= _statusHeight();
  if(allocation.width != lastSize.width || allocation.height != lastSize.height) {
    auto size = self().geometry().size();
    state().geometry.setSize({allocation.width, allocation.height});
    if(auto& sizable = state().sizable) {
      sizable->setGeometry(self().geometry().setPosition());
    }
    self().doSize();

    //for GTK3: the window will not update after changing widget sizes until sending size-allocate
    //size-allocate will also call _synchronizeMargin() which is also important for window sizing
    //GtkAllocation is a typedef of GdkRectangle
    GtkAllocation rectangle;
    gtk_widget_get_allocation(widget, &rectangle);
    g_signal_emit_by_name(G_OBJECT(widget), "size-allocate", &rectangle, (gpointer)this, nullptr);
  }
  lastSize = allocation;

  auto gdkWindow = gtk_widget_get_window(widget);
  gdk_window_get_origin(gdkWindow, &allocation.x, &allocation.y);
  allocation.y += _menuHeight();
  if(allocation.x != lastMove.x || allocation.y != lastMove.y) {
    state().geometry.setPosition({allocation.x, allocation.y});
    self().doMove();
  }
  lastMove = allocation;
}

auto pWindow::_synchronizeMargin() -> void {
  if(locked()) return;
  auto lock = acquire();

  if(!gtk_widget_get_realized(widget)) return;
  if(!gtk_widget_get_visible(widget)) return;
  if(state().fullScreen || state().maximized || state().minimized) return;

  auto window = gtk_widget_get_window(widget);
  GdkRectangle border, client;
  gdk_window_get_frame_extents(window, &border);
  gdk_window_get_origin(window, &client.x, &client.y);
  #if HIRO_GTK==2
  gdk_window_get_geometry(window, nullptr, nullptr, &client.width, &client.height, nullptr);
  #elif HIRO_GTK==3
  gdk_window_get_geometry(window, nullptr, nullptr, &client.width, &client.height);
  #endif

  settings.geometry.frameX = client.x - border.x;
  settings.geometry.frameY = client.y - border.y;
  settings.geometry.frameWidth  = border.width  - client.width;
  settings.geometry.frameHeight = border.height - client.height;

  if(gtk_widget_get_visible(gtkMenu)) {
    GtkAllocation allocation;
    auto time = chrono::millisecond();
    while(chrono::millisecond() - time < 20) {
      gtk_widget_get_allocation(gtkMenu, &allocation);
      if(allocation.height > 1) {
        settings.geometry.menuHeight = allocation.height - _menuTextHeight();
        break;
      }
    }
  }

  if(gtk_widget_get_visible(gtkStatus)) {
    GtkAllocation allocation;
    auto time = chrono::millisecond();
    while(chrono::millisecond() - time < 20) {
      gtk_widget_get_allocation(gtkStatus, &allocation);
      if(allocation.height > 1) {
        settings.geometry.statusHeight = allocation.height - _statusTextHeight();
        break;
      }
    }
  }
}

//GTK doesn't add gtk_window_is_maximized() until 3.12;
//and doesn't appear to have a companion gtk_window_is_(hidden,iconic,minimized);
//so we have to do this the hard way
auto pWindow::_synchronizeState() -> void {
  if(locked()) return;
  auto lock = acquire();

  if(!gtk_widget_get_realized(widget)) return;

  #if defined(DISPLAY_WINDOWS)
  auto window = (HWND)GDK_WINDOW_HWND(gtk_widget_get_window(widget));

  bool maximized = IsZoomed(window);
  bool minimized = IsIconic(window);

  bool doSize = false;
  if(state().minimized != minimized) doSize = true;

  state().maximized = maximized;
  state().minimized = minimized;

  if(doSize) self().doSize();
  #endif

  #if defined(DISPLAY_XORG)
  auto display = XOpenDisplay(nullptr);
  int screen = DefaultScreen(display);
  auto window = GDK_WINDOW_XID(gtk_widget_get_window(widget));
  XlibAtom wmState = XInternAtom(display, "_NET_WM_STATE", XlibTrue);
  XlibAtom atom;
  int format;
  unsigned long items, after;
  unsigned char* data = nullptr;
  int result = XGetWindowProperty(
    display, window, wmState, 0, LONG_MAX, XlibFalse, AnyPropertyType, &atom, &format, &items, &after, &data
  );
  auto atoms = (unsigned long*)data;
  if(result == Success) {
    bool maximizedHorizontal = false;
    bool maximizedVertical = false;
    bool minimized = false;

    for(auto index : range(items)) {
      auto memory = XGetAtomName(display, atoms[index]);
      auto name = string{memory};
      if(name == "_NET_WM_STATE_MAXIMIZED_HORZ") maximizedHorizontal = true;
      if(name == "_NET_WM_STATE_MAXIMIZED_VERT") maximizedVertical = true;
      if(name == "_NET_WM_STATE_HIDDEN") minimized = true;
      XFree(memory);
    }

    bool doSize = false;
    //maximize sends size-allocate, which triggers doSize()
    if(state().minimized != minimized) doSize = true;

    //windows do not act bizarrely when maximized in only one direction
    //so for this reason, consider a window maximized only if it's in both directions
    state().maximized = maximizedHorizontal && maximizedVertical;
    state().minimized = minimized;

    if(doSize) self().doSize();
  }
  XCloseDisplay(display);
  #endif
}

}

#endif
