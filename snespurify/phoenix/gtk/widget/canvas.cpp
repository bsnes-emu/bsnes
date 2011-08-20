static gboolean Canvas_expose(GtkWidget *widget, GdkEvent *event, pCanvas *self) {
  cairo_t *context = gdk_cairo_create(gtk_widget_get_window(widget));
  cairo_set_source_surface(context, self->surface, 0, 0);
  cairo_paint(context);
  cairo_destroy(context);
  return true;
}

uint32_t* pCanvas::buffer() {
  return (uint32_t*)cairo_image_surface_get_data(surface);
}

void pCanvas::setGeometry(const Geometry &geometry) {
  if(geometry.width  == cairo_image_surface_get_width(surface)
  && geometry.height == cairo_image_surface_get_height(surface)) return;

  cairo_surface_destroy(surface);
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, geometry.width, geometry.height);

  pWidget::setGeometry(geometry);
  update();
}

void pCanvas::update() {
  if(gtk_widget_get_realized(gtkWidget) == false) return;
  gdk_window_invalidate_rect(gtk_widget_get_window(gtkWidget), 0, true);
}

void pCanvas::constructor() {
  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 256, 256);
  gtkWidget = gtk_drawing_area_new();
  GdkColor color;
  color.pixel = color.red = color.green = color.blue = 0;
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget, GDK_EXPOSURE_MASK);
  g_signal_connect(G_OBJECT(gtkWidget), "expose_event", G_CALLBACK(Canvas_expose), (gpointer)this);
}
