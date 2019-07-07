#if defined(Hiro_Canvas)

namespace hiro {

//GTK3
static auto Canvas_draw(GtkWidget* widget, cairo_t* context, pCanvas* p) -> signed {
  p->_onDraw(context);
  return true;
}

//GTK2
static auto Canvas_expose(GtkWidget* widget, GdkEventExpose* event, pCanvas* p) -> signed {
  p->_onExpose(event);
  return true;
}

static auto Canvas_keyPress(GtkWidget* widget, GdkEventKey* event, pViewport* p) -> signed {
  //canvass that have been set focusable are intended for games.
  //to prevent arrow keys, tab, etc from losing focus on the game viewport, block key propagation here.
  if(p->self().focusable()) return true;

  //for all other cases, allow the key to propagate.
  return false;
}

static auto Canvas_mousePress(GtkWidget* widget, GdkEventButton* event, pCanvas* p) -> signed {
  //gtk_widget_set_focus_on_click() is a GTK 3.2+ feature.
  //implement this functionality manually for GTK 2.0+ compatibility.
  if(event->button == 1 && p->self().focusable()) gtk_widget_grab_focus(widget);

  return Widget_mousePress(widget, event, p);
}

auto pCanvas::construct() -> void {
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);

  _rasterize();
  _redraw();

  //todo: need to work around GTK+ library bug:
  //after calling destruct(), construct() with state.droppable == true;
  //GTK+ will throw SIGBUS inside g_signal_connect_data() on one of the below connections

  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Canvas_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Widget_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Widget_drop), (gpointer)this);
  #if HIRO_GTK==2
  g_signal_connect(G_OBJECT(gtkWidget), "expose-event", G_CALLBACK(Canvas_expose), (gpointer)this);
  #elif HIRO_GTK==3
  g_signal_connect(G_OBJECT(gtkWidget), "draw", G_CALLBACK(Canvas_draw), (gpointer)this);
  #endif
  g_signal_connect(G_OBJECT(gtkWidget), "key-press-event", G_CALLBACK(Canvas_keyPress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "enter-notify-event", G_CALLBACK(Widget_mouseEnter), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Widget_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion-notify-event", G_CALLBACK(Widget_mouseMove), (gpointer)this);

  pWidget::construct();
}

auto pCanvas::destruct() -> void {
  _release();
  if(gtkWidget) gtk_widget_destroy(gtkWidget), gtkWidget = nullptr;
  gtkParent = nullptr;
}

auto pCanvas::handle() const -> uintptr {
  #if defined(DISPLAY_WINDOWS)
  return (uintptr)GDK_WINDOW_HWND(gtk_widget_get_window(gtkWidget));
  #endif

  #if defined(DISPLAY_XORG)
  return (uintptr)GDK_WINDOW_XID(gtk_widget_get_window(gtkWidget));
  #endif

  return (uintptr)nullptr;
}

auto pCanvas::minimumSize() const -> Size {
  if(auto& icon = state().icon) return {(int)icon.width(), (int)icon.height()};
  return {0, 0};
}

auto pCanvas::setAlignment(Alignment) -> void {
  update();
}

auto pCanvas::setColor(Color color) -> void {
  update();
}

auto pCanvas::setFocusable(bool focusable) -> void {
  gtk_widget_set_can_focus(gtkWidget, focusable);
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  update();
  pWidget::setGeometry(geometry);
}

auto pCanvas::setGradient(Gradient gradient) -> void {
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  _redraw();
}

auto pCanvas::_onDraw(cairo_t* context) -> void {
  if(!surface) return;

  int sx = 0, sy = 0, dx = 0, dy = 0;
  int width = surfaceWidth, height = surfaceHeight;
  auto geometry = pSizable::state().geometry;
  auto alignment = state().alignment ? state().alignment : Alignment{0.5, 0.5};

  if(width <= geometry.width()) {
    sx = 0;
    dx = (geometry.width() - width) * alignment.horizontal();
  } else {
    sx = (width - geometry.width()) * alignment.horizontal();
    dx = 0;
  }

  if(height <= geometry.height()) {
    sy = 0;
    dy = (geometry.height() - height) * alignment.vertical();
  } else {
    sy = (height - geometry.height()) * alignment.vertical();
    dy = 0;
  }

  cairo_set_source_rgba(context, 0.0, 0.0, 0.0, 0.0);
  cairo_paint(context);
  gdk_cairo_set_source_pixbuf(context, surface, dx - sx, dy - sy);
  cairo_rectangle(context, dx, dy, width, height);
  cairo_paint(context);
}

auto pCanvas::_onExpose(GdkEventExpose* expose) -> void {
  if(!surface) return;

  cairo_t* context = gdk_cairo_create(gtk_widget_get_window(gtkWidget));
  _onDraw(context);
  cairo_destroy(context);
}

auto pCanvas::_rasterize() -> void {
  int width = 0;
  int height = 0;

  if(auto& icon = state().icon) {
    width = icon.width();
    height = icon.height();
  } else {
    width = pSizable::state().geometry.width();
    height = pSizable::state().geometry.height();
  }
  if(width <= 0 || height <= 0) return;

  if(width != surfaceWidth || height != surfaceHeight) _release();
  surfaceWidth = width;
  surfaceHeight = height;

  if(!surface) surface = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height);
  auto buffer = (uint32_t*)gdk_pixbuf_get_pixels(surface);

  if(auto& icon = state().icon) {
    memory::copy<uint32_t>(buffer, state().icon.data(), width * height);
  } else if(auto& gradient = state().gradient) {
    auto& colors = gradient.state.colors;
    image fill;
    fill.allocate(width, height);
    fill.gradient(colors[0].value(), colors[1].value(), colors[2].value(), colors[3].value());
    memory::copy(buffer, fill.data(), fill.size());
  } else {
    uint32_t color = state().color.value();
    for(auto n : range(width * height)) buffer[n] = color;
  }

  //ARGB -> ABGR conversion
  for(auto n : range(width * height)) {
    uint32_t color = *buffer;
    color = (color & 0xff00ff00) | ((color & 0xff0000) >> 16) | ((color & 0x0000ff) << 16);
    *buffer++ = color;
  }
}

auto pCanvas::_redraw() -> void {
  if(gtk_widget_get_realized(gtkWidget)) {
    gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), nullptr, true);
  }
}

auto pCanvas::_release() -> void {
  if(surface) {
    g_object_unref(surface);
    surface = nullptr;
  }
  surfaceWidth = 0;
  surfaceHeight = 0;
}

}

#endif
