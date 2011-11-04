static gboolean Canvas_expose(GtkWidget *widget, GdkEvent *event, pCanvas *self) {
  cairo_t *context = gdk_cairo_create(gtk_widget_get_window(widget));
  cairo_set_source_surface(context, self->surface, 0, 0);
  cairo_paint(context);
  cairo_destroy(context);
  return true;
}

void pCanvas::setSize(const Size &size) {
  cairo_surface_destroy(surface);
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, canvas.state.width, canvas.state.height);
}

void pCanvas::update() {
  memcpy(cairo_image_surface_get_data(surface), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  if(gtk_widget_get_realized(gtkWidget) == false) return;
  gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), 0, true);
}

void pCanvas::constructor() {
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, canvas.state.width, canvas.state.height);
  memcpy(cairo_image_surface_get_data(surface), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));
  gtkWidget = gtk_drawing_area_new();
  gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget, GDK_EXPOSURE_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "expose_event", G_CALLBACK(Canvas_expose), (gpointer)this);
}

void pCanvas::destructor() {
  gtk_widget_destroy(gtkWidget);
  cairo_surface_destroy(surface);
}

void pCanvas::orphan() {
  destructor();
  constructor();
}
