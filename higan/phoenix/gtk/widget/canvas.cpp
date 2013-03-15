namespace phoenix {

static gboolean Canvas_expose(GtkWidget *widget, GdkEvent *event, pCanvas *self) {
  cairo_t *context = gdk_cairo_create(gtk_widget_get_window(widget));
  cairo_set_source_surface(context, self->surface, 0, 0);
  cairo_paint(context);
  cairo_destroy(context);
  return true;
}

static gboolean Canvas_mouseLeave(GtkWidget *widget, GdkEventButton *event, pCanvas *self) {
  if(self->canvas.onMouseLeave) self->canvas.onMouseLeave();
  return true;
}

static gboolean Canvas_mouseMove(GtkWidget *widget, GdkEventButton *event, pCanvas *self) {
  if(self->canvas.onMouseMove) self->canvas.onMouseMove({ (signed)event->x, (signed)event->y });
  return true;
}

static gboolean Canvas_mousePress(GtkWidget *widget, GdkEventButton *event, pCanvas *self) {
  if(self->canvas.onMousePress) switch(event->button) {
  case 1: self->canvas.onMousePress(Mouse::Button::Left); break;
  case 2: self->canvas.onMousePress(Mouse::Button::Middle); break;
  case 3: self->canvas.onMousePress(Mouse::Button::Right); break;
  }
  return true;
}

static gboolean Canvas_mouseRelease(GtkWidget *widget, GdkEventButton *event, pCanvas *self) {
  if(self->canvas.onMouseRelease) switch(event->button) {
  case 1: self->canvas.onMouseRelease(Mouse::Button::Left); break;
  case 2: self->canvas.onMouseRelease(Mouse::Button::Middle); break;
  case 3: self->canvas.onMouseRelease(Mouse::Button::Right); break;
  }
  return true;
}

void pCanvas::setSize(const Size &size) {
  cairo_surface_destroy(surface);
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, canvas.state.width, canvas.state.height);
}

void pCanvas::update() {
  memcpy(cairo_image_surface_get_data(surface), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  if(gtk_widget_get_realized(gtkWidget) == false) return;
  gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), nullptr, true);
}

void pCanvas::constructor() {
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, canvas.state.width, canvas.state.height);
  memcpy(cairo_image_surface_get_data(surface), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget,
    GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "button_press_event", G_CALLBACK(Canvas_mousePress), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "button_release_event", G_CALLBACK(Canvas_mouseRelease), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "expose_event", G_CALLBACK(Canvas_expose), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "leave_notify_event", G_CALLBACK(Canvas_mouseLeave), (gpointer)this);
  g_signal_connect(G_OBJECT(gtkWidget), "motion_notify_event", G_CALLBACK(Canvas_mouseMove), (gpointer)this);
}

void pCanvas::destructor() {
  gtk_widget_destroy(gtkWidget);
  cairo_surface_destroy(surface);
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

}
