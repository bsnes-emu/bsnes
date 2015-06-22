#if defined(Hiro_Viewport)

namespace hiro {

static auto Viewport_dropEvent(GtkWidget* widget, GdkDragContext* context, signed x, signed y,
GtkSelectionData* data, unsigned type, unsigned timestamp, pViewport* p) -> void {
  if(!p->state().droppable) return;
  lstring paths = DropPaths(data);
  if(paths.empty()) return;
  p->self().doDrop(paths);
}

static auto Viewport_mouseLeave(GtkWidget* widget, GdkEventButton* event, pViewport* p) -> signed {
  p->self().doMouseLeave();
  return true;
}

static auto Viewport_mouseMove(GtkWidget* widget, GdkEventButton* event, pViewport* p) -> signed {
  p->self().doMouseMove({(signed)event->x, (signed)event->y});
  return true;
}

static auto Viewport_mousePress(GtkWidget* widget, GdkEventButton* event, pViewport* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMousePress(Mouse::Button::Left); break;
  case 2: p->self().doMousePress(Mouse::Button::Middle); break;
  case 3: p->self().doMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static auto Viewport_mouseRelease(GtkWidget* widget, GdkEventButton* event, pViewport* p) -> signed {
  switch(event->button) {
  case 1: p->self().doMouseRelease(Mouse::Button::Left); break;
  case 2: p->self().doMouseRelease(Mouse::Button::Middle); break;
  case 3: p->self().doMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

auto pViewport::construct() -> void {
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);

  GdkColor color;
  color.pixel = 0;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, &color);

  setDroppable(state().droppable);

  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Viewport_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Viewport_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Viewport_dropEvent), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Viewport_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion-notify-event", G_CALLBACK(Viewport_mouseMove), (gpointer)this);

  pWidget::construct();
}

auto pViewport::destruct() -> void {
  if(gtkWidget) gtk_widget_destroy(gtkWidget), gtkWidget = nullptr;
  gtkParent = nullptr;
}

auto pViewport::handle() const -> uintptr_t {
  #if defined(DISPLAY_WINDOWS)
  return (uintptr_t)GDK_WINDOW_HWND(gtk_widget_get_window(gtkWidget));
  #endif

  #if defined(DISPLAY_XORG)
  return GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
  #endif
}

auto pViewport::setDroppable(bool droppable) -> void {
  if(droppable) {
    gtk_drag_dest_set(gtkWidget, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
    gtk_drag_dest_add_uri_targets(gtkWidget);
  }
}

}

#endif
