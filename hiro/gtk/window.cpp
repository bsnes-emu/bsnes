#if defined(Hiro_Window)

namespace hiro {

static auto Window_close(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  if(p->state().onClose) {
    p->self().doClose();
  } else {
    p->self().setVisible(false);
  }
  if(p->state().modal && !p->pObject::state().visible) p->self().setModal(false);
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
    cairo_destroy(context);
  }
  return false;
}

//GTK2 expose-event
static auto Window_expose(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  if(auto color = p->state().backgroundColor) {
    cairo_t* context = gdk_cairo_create(gtk_widget_get_window(widget));
    return Window_draw(widget, context, p);
  }
  return false;
}

static auto Window_configure(GtkWidget* widget, GdkEvent* event, pWindow* p) -> signed {
  if(!gtk_widget_get_realized(p->widget)) return false;
  if(!p->pObject::state().visible) return false;
  GdkWindow* gdkWindow = gtk_widget_get_window(widget);

  GdkRectangle border, client;
  gdk_window_get_frame_extents(gdkWindow, &border);
  #if HIRO_GTK==2
  gdk_window_get_geometry(gdkWindow, nullptr, nullptr, &client.width, &client.height, nullptr);
  #elif HIRO_GTK==3
  gdk_window_get_geometry(gdkWindow, nullptr, nullptr, &client.width, &client.height);
  #endif
  gdk_window_get_origin(gdkWindow, &client.x, &client.y);

  if(!p->state().fullScreen) {
    //update geometry settings
    settings.geometry.frameX = client.x - border.x;
    settings.geometry.frameY = client.y - border.y;
    settings.geometry.frameWidth = border.width - client.width;
    settings.geometry.frameHeight = border.height - client.height;
  }

  Geometry geometry = {
    client.x,
    client.y + p->_menuHeight(),
    client.width,
    client.height - p->_menuHeight() - p->_statusHeight()
  };

  //move
  if(geometry.x() != p->state().geometry.x() || geometry.y() != p->state().geometry.y()) {
    if(!p->state().fullScreen) {
      p->state().geometry.setPosition({geometry.x(), geometry.y()});
    }
    if(!p->locked()) p->self().doMove();
  }

  //size
  if(geometry.width() != p->state().geometry.width() || geometry.height() != p->state().geometry.height()) {
    p->onSizePending = true;
  }

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
  //TODO: get pWindow; use sizeRequest
}

static auto Window_getPreferredHeight(GtkWidget* widget, int* minimalHeight, int* naturalHeight) -> void {
  //TODO: get pWindow; use sizeRequest
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

static auto Window_sizeAllocate(GtkWidget* widget, GtkAllocation* allocation, pWindow* p) -> void {
  //size-allocate sent from gtk_fixed_move(); detect if layout unchanged and return
  if(allocation->width == p->lastAllocation.width
  && allocation->height == p->lastAllocation.height) return;

  if(!p->state().fullScreen) {
    p->state().geometry.setSize({allocation->width, allocation->height});
  }

  if(auto& layout = p->state().layout) {
    layout->setGeometry(p->self().geometry().setPosition(0, 0));
  }

  if(!p->locked() && p->onSizePending) {
    p->onSizePending = false;
    p->self().doSize();
  }

  p->lastAllocation = *allocation;
}

static auto Window_sizeRequest(GtkWidget* widget, GtkRequisition* requisition, pWindow* p) -> void {
  requisition->width  = p->state().geometry.width();
  requisition->height = p->state().geometry.height();
}

auto pWindow::construct() -> void {
  lastAllocation.width  = 0;
  lastAllocation.height = 0;
  onSizePending = false;

  widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_resizable(GTK_WINDOW(widget), true);

  //if program was given a name, try and set the window taskbar icon from one of the pixmaps folders
  if(!Application::state.name);
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
  g_signal_connect(G_OBJECT(formContainer), "size-allocate", G_CALLBACK(Window_sizeAllocate), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(formContainer), "size-request", G_CALLBACK(Window_sizeRequest), (gpointer)this);
  #elif HIRO_GTK==3
  auto widgetClass = GTK_WIDGET_GET_CLASS(formContainer);
  widgetClass->get_preferred_width = Window_getPreferredWidth;
  widgetClass->get_preferred_height = Window_getPreferredHeight;
  #endif
}

auto pWindow::destruct() -> void {
  gtk_widget_destroy(widget);
}

auto pWindow::append(sLayout layout) -> void {
}

auto pWindow::append(sMenuBar menuBar) -> void {
  _setMenuEnabled(menuBar->enabled(true));
  _setMenuFont(menuBar->font(true));
  _setMenuVisible(menuBar->visible(true));
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

auto pWindow::remove(sLayout layout) -> void {
}

auto pWindow::remove(sMenuBar menuBar) -> void {
  _setMenuVisible(false);
}

auto pWindow::remove(sStatusBar statusBar) -> void {
  _setStatusVisible(false);
}

auto pWindow::setBackgroundColor(Color color) -> void {
  GdkColor gdkColor = CreateColor(color);
  gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, color ? &gdkColor : nullptr);
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

  if(auto& layout = state().layout) {
    if(auto self = layout->self()) self->setEnabled(layout->enabled(true));
  }
}

auto pWindow::setFocused() -> void {
  gtk_window_present(GTK_WINDOW(widget));
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  if(fullScreen) {
    windowedGeometry = state().geometry;
    gtk_window_fullscreen(GTK_WINDOW(widget));
    state().geometry = Monitor::geometry(Monitor::primary());
  } else {
    gtk_window_unfullscreen(GTK_WINDOW(widget));
    state().geometry = windowedGeometry;
  }
  auto time = chrono::millisecond();
  while(chrono::millisecond() - time < 20) gtk_main_iteration_do(false);
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  Geometry margin = frameMargin();
  gtk_window_move(GTK_WINDOW(widget), geometry.x() - margin.x(), geometry.y() - margin.y());

  GdkGeometry geom;
  geom.min_width  = state().resizable ? 1 : state().geometry.width();
  geom.min_height = state().resizable ? 1 : state().geometry.height();
  gtk_window_set_geometry_hints(GTK_WINDOW(widget), GTK_WIDGET(widget), &geom, GDK_HINT_MIN_SIZE);

  gtk_widget_set_size_request(formContainer, geometry.width(), geometry.height());
  auto time1 = chrono::millisecond();
  while(chrono::millisecond() - time1 < 20) gtk_main_iteration_do(false);

  gtk_window_resize(GTK_WINDOW(widget), geometry.width(), geometry.height() + _menuHeight() + _statusHeight());
  auto time2 = chrono::millisecond();
  while(chrono::millisecond() - time2 < 20) gtk_main_iteration_do(false);
}

auto pWindow::setModal(bool modal) -> void {
  if(modal) {
    gtk_window_set_modal(GTK_WINDOW(widget), true);
    while(!Application::state.quit && state().modal) {
      if(Application::state.onMain) {
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
}

auto pWindow::setTitle(const string& title) -> void {
  gtk_window_set_title(GTK_WINDOW(widget), title ? title : " ");
}

auto pWindow::setVisible(bool visible) -> void {
  gtk_widget_set_visible(widget, visible);

  if(auto& menuBar = state().menuBar) {
    if(menuBar->self()) menuBar->self()->setVisible(menuBar->visible(true));
  }

  if(auto& layout = state().layout) {
    if(layout->self()) layout->self()->setVisible(layout->visible(true));
  }

  if(auto& statusBar = state().statusBar) {
    if(statusBar->self()) statusBar->self()->setVisible(statusBar->visible(true));
  }

  //GTK+ returns invalid widget sizes below without this call
  Application::processEvents();

  if(visible) {
    if(gtk_widget_get_visible(gtkMenu)) {
      GtkAllocation allocation;
      gtk_widget_get_allocation(gtkMenu, &allocation);
      settings.geometry.menuHeight = allocation.height;
    }

    if(gtk_widget_get_visible(gtkStatus)) {
      GtkAllocation allocation;
      gtk_widget_get_allocation(gtkStatus, &allocation);
      settings.geometry.statusHeight = allocation.height;
    }
  }

  if(auto& layout = state().layout) {
    layout->setGeometry(self().geometry().setPosition(0, 0));
  }
}

auto pWindow::_append(mWidget& widget) -> void {
  if(!widget.self()) return;
  if(auto parent = widget.parentWidget(true)) {
    if(auto instance = parent->self()) widget.self()->gtkParent = instance->container(widget);
  } else {
    widget.self()->gtkParent = formContainer;
  }
  gtk_fixed_put(GTK_FIXED(widget.self()->gtkParent), widget.self()->gtkWidget, 0, 0);
}

auto pWindow::_append(mMenu& menu) -> void {
  if(menu.self()) gtk_menu_shell_append(GTK_MENU_SHELL(gtkMenu), menu.self()->widget);
}

auto pWindow::_menuHeight() const -> signed {
  return gtk_widget_get_visible(gtkMenu) ? settings.geometry.menuHeight : 0;
}

auto pWindow::_setIcon(const string& pathname) -> bool {
  string filename;

  filename = {pathname, Application::state.name, ".svg"};
  if(file::exists(filename)) {
    gtk_window_set_icon_from_file(GTK_WINDOW(widget), filename, nullptr);
    return true;
  }

  filename = {pathname, Application::state.name, ".png"};
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

auto pWindow::_statusHeight() const -> signed {
  return gtk_widget_get_visible(gtkStatus) ? settings.geometry.statusHeight : 0;
}

}

#endif
