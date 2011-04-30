static void Canvas_expose(pCanvas *self) {
  self->redraw();
}

uint32_t* pCanvas::buffer() {
  return bufferRGB;
}

void pCanvas::setGeometry(const Geometry &geometry) {
  delete[] bufferRGB;
  delete[] bufferBGR;

  bufferRGB = new uint32_t[geometry.width * geometry.height]();
  bufferBGR = new uint32_t[geometry.width * geometry.height]();

  pWidget::setGeometry(geometry);
  update();
}

void pCanvas::update() {
  if(gtk_widget_get_realized(gtkWidget) == false) return;
  GdkRectangle rect;
  rect.x = 0;
  rect.y = 0;
  rect.width = gtkWidget->allocation.width;
  rect.height = gtkWidget->allocation.height;
  gdk_window_invalidate_rect(gtkWidget->window, &rect, true);
}

void pCanvas::constructor() {
  bufferRGB = new uint32_t[256 * 256]();
  bufferBGR = new uint32_t[256 * 256]();

  gtkWidget = gtk_drawing_area_new();
  GdkColor color;
  color.pixel = color.red = color.green = color.blue = 0;
  gtk_widget_modify_bg(gtkWidget, GTK_STATE_NORMAL, &color);
  gtk_widget_set_double_buffered(gtkWidget, false);
  gtk_widget_add_events(gtkWidget, GDK_EXPOSURE_MASK);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "expose_event", G_CALLBACK(Canvas_expose), (gpointer)this);
}

void pCanvas::redraw() {
  if(gtk_widget_get_realized(gtkWidget) == false) return;
  uint32_t *rgb = bufferRGB, *bgr = bufferBGR;
  for(unsigned y = gtkWidget->allocation.height; y; y--) {
    for(unsigned x = gtkWidget->allocation.width; x; x--) {
      uint32_t pixel = *rgb++;
      *bgr++ = ((pixel << 16) & 0xff0000) | (pixel & 0x00ff00) | ((pixel >> 16) & 0x0000ff);
    }
  }

  gdk_draw_rgb_32_image(
    gtkWidget->window,
    gtkWidget->style->fg_gc[GTK_WIDGET_STATE(gtkWidget)],
    0, 0, gtkWidget->allocation.width, gtkWidget->allocation.height,
    GDK_RGB_DITHER_NONE, (guchar*)bufferBGR, sizeof(uint32_t) * gtkWidget->allocation.width
  );
}
