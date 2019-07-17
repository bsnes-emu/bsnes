#if defined(Hiro_Viewport)

namespace hiro {

//GTK3
static auto Viewport_draw(GtkWidget* widget, cairo_t* context, pViewport* p) -> signed {
  p->_onDraw(context);
  return true;
}

//GTK2
static auto Viewport_expose(GtkWidget* widget, GdkEventExpose* event, pViewport* p) -> signed {
  p->_onExpose(event);
  return true;
}

static auto Viewport_keyPress(GtkWidget* widget, GdkEventKey* event, pViewport* p) -> signed {
  //viewports that have been set focusable are intended for games.
  //to prevent arrow keys, tab, etc from losing focus on the game viewport, block key propagation here.
  if(p->self().focusable()) return true;

  //for all other cases, allow the key to propagate.
  return false;
}

static auto Viewport_mousePress(GtkWidget* widget, GdkEventButton* event, pViewport* p) -> signed {
  //gtk_widget_set_focus_on_click() is a GTK 3.2+ feature.
  //implement this functionality manually for GTK 2.0+ compatibility.
  if(event->button == 1 && p->self().focusable()) gtk_widget_grab_focus(widget);

  return Widget_mousePress(widget, event, p);
}

auto pViewport::construct() -> void {
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);

  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Viewport_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Widget_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Widget_drop), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(gtkWidget), "expose-event", G_CALLBACK(Viewport_expose), (gpointer)this);
  #elif HIRO_GTK==3
  g_signal_connect(G_OBJECT(gtkWidget), "draw", G_CALLBACK(Viewport_draw), (gpointer)this);
  #endif
  g_signal_connect(G_OBJECT(gtkWidget), "key-press-event", G_CALLBACK(Viewport_keyPress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "enter-notify-event", G_CALLBACK(Widget_mouseEnter), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Widget_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion-notify-event", G_CALLBACK(Widget_mouseMove), (gpointer)this);

  pWidget::construct();
}

auto pViewport::destruct() -> void {
  if(gtkWidget) gtk_widget_destroy(gtkWidget), gtkWidget = nullptr;
  gtkParent = nullptr;
}

auto pViewport::handle() const -> uintptr {
  #if defined(DISPLAY_WINDOWS)
  return (uintptr)GDK_WINDOW_HWND(gtk_widget_get_window(gtkWidget));
  #endif

  #if defined(DISPLAY_XORG)
  return (uintptr)GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
  #endif

  return (uintptr)nullptr;
}

auto pViewport::setFocusable(bool focusable) -> void {
  gtk_widget_set_can_focus(gtkWidget, focusable);
}

auto pViewport::_onDraw(cairo_t* context) -> void {
  cairo_set_source_rgba(context, 0.0, 0.0, 0.0, 1.0);
  cairo_paint(context);
}

auto pViewport::_onExpose(GdkEventExpose* expose) -> void {
  cairo_t* context = gdk_cairo_create(gtk_widget_get_window(gtkWidget));
  _onDraw(context);
  cairo_destroy(context);
}

}

#endif
