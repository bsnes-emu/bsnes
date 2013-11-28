namespace phoenix {

static void Canvas_drop(GtkWidget* widget, GdkDragContext* context, signed x, signed y,
GtkSelectionData* data, unsigned type, unsigned timestamp, Canvas* canvas) {
  if(!canvas->state.droppable) return;
  lstring paths = DropPaths(data);
  if(paths.empty()) return;
  if(canvas->onDrop) canvas->onDrop(paths);
}

static signed Canvas_expose(GtkWidget* widget, GdkEventExpose* event, Canvas* self) {
  self->p.onExpose(event);
  return true;
}

static signed Canvas_mouseLeave(GtkWidget* widget, GdkEventButton* event, Canvas* self) {
  if(self->onMouseLeave) self->onMouseLeave();
  return true;
}

static signed Canvas_mouseMove(GtkWidget* widget, GdkEventButton* event, Canvas* self) {
  if(self->onMouseMove) self->onMouseMove({(signed)event->x, (signed)event->y});
  return true;
}

static signed Canvas_mousePress(GtkWidget* widget, GdkEventButton* event, Canvas* self) {
  if(self->onMousePress) switch(event->button) {
  case 1: self->onMousePress(Mouse::Button::Left); break;
  case 2: self->onMousePress(Mouse::Button::Middle); break;
  case 3: self->onMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static signed Canvas_mouseRelease(GtkWidget* widget, GdkEventButton* event, Canvas* self) {
  if(self->onMouseRelease) switch(event->button) {
  case 1: self->onMouseRelease(Mouse::Button::Left); break;
  case 2: self->onMouseRelease(Mouse::Button::Middle); break;
  case 3: self->onMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

Size pCanvas::minimumSize() {
  return {canvas.state.width, canvas.state.height};
}

void pCanvas::setDroppable(bool droppable) {
  gtk_drag_dest_set(gtkWidget, GTK_DEST_DEFAULT_ALL, nullptr, 0, GDK_ACTION_COPY);
  if(droppable) gtk_drag_dest_add_uri_targets(gtkWidget);
}

void pCanvas::setGeometry(Geometry geometry) {
  if(canvas.state.width == 0 || canvas.state.height == 0) rasterize();
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width < geometry.width) {
    geometry.x += (geometry.width - width) / 2;
    geometry.width = width;
  }

  if(height < geometry.height) {
    geometry.y += (geometry.height - height) / 2;
    geometry.height = height;
  }

  pWidget::setGeometry(geometry);
}

void pCanvas::setMode(Canvas::Mode mode) {
  rasterize(), redraw();
}

void pCanvas::setSize(Size size) {
  rasterize(), redraw();
}

void pCanvas::constructor() {
  gtkWidget = gtk_drawing_area_new();
//gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "button-press-event", G_CALLBACK(Canvas_mousePress), (gpointer)&canvas);
  g_signal_connect(G_OBJECT(gtkWidget), "button-release-event", G_CALLBACK(Canvas_mouseRelease), (gpointer)&canvas);
  g_signal_connect(G_OBJECT(gtkWidget), "drag-data-received", G_CALLBACK(Canvas_drop), (gpointer)&canvas);
  g_signal_connect(G_OBJECT(gtkWidget), "expose-event", G_CALLBACK(Canvas_expose), (gpointer)&canvas);
  g_signal_connect(G_OBJECT(gtkWidget), "leave-notify-event", G_CALLBACK(Canvas_mouseLeave), (gpointer)&canvas);
  g_signal_connect(G_OBJECT(gtkWidget), "motion-notify-event", G_CALLBACK(Canvas_mouseMove), (gpointer)&canvas);

  setDroppable(canvas.state.droppable);
  rasterize(), redraw();
}

void pCanvas::destructor() {
  release();
  gtk_widget_destroy(gtkWidget);
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::onExpose(GdkEventExpose* expose) {
  if(surface) gdk_draw_pixbuf(gtk_widget_get_window(gtkWidget), nullptr, surface, 0, 0, 0, 0, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
}

void pCanvas::rasterize() {
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width != surfaceWidth || height != surfaceHeight) release();
  surfaceWidth = width;
  surfaceHeight = height;

  if(width == 0 || height == 0) return;

  if(!surface) surface = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height);
  uint32_t* buffer = (uint32_t*)gdk_pixbuf_get_pixels(surface);

  if(canvas.state.mode == Canvas::Mode::Color) {
    uint32_t color = canvas.state.color.argb();
    for(unsigned n = 0; n < width * height; n++) buffer[n] = color;
  }

  if(canvas.state.mode == Canvas::Mode::Gradient) {
    nall::image image;
    image.allocate(width, height);
    image.gradient(
      canvas.state.gradient[0].argb(), canvas.state.gradient[1].argb(), canvas.state.gradient[2].argb(), canvas.state.gradient[3].argb()
    );
    memcpy(buffer, image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Image) {
    nall::image image = canvas.state.image;
    image.scale(width, height);
    image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    memcpy(buffer, image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Data) {
    if(width == canvas.state.width && height == canvas.state.height) {
      memcpy(buffer, canvas.state.data, width * height * sizeof(uint32_t));
    } else {
      memset(buffer, 0x00, width * height * sizeof(uint32_t));
    }
  }

  //ARGB -> ABGR conversion
  for(unsigned n = 0; n < width * height; n++) {
    uint32_t color = *buffer;
    color = (color & 0xff00ff00) | ((color & 0xff0000) >> 16) | ((color & 0x0000ff) << 16);
    *buffer++ = color;
  }
}

void pCanvas::redraw() {
  if(gtk_widget_get_realized(gtkWidget)) {
    gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), nullptr, true);
  }
}

void pCanvas::release() {
  if(surface == nullptr) return;
  g_object_unref(surface);
  surface = nullptr;
  surfaceWidth = 0;
  surfaceHeight = 0;
}

}
