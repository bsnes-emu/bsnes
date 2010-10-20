static void Canvas_expose(Canvas *self) {
  uint32_t *rgb = self->canvas->bufferRGB;
  uint32_t *bgr = self->canvas->bufferBGR;
  for(unsigned y = self->object->widget->allocation.height; y; y--) {
    for(unsigned x = self->object->widget->allocation.width; x; x--) {
      uint32_t pixel = *rgb++;
      *bgr++ = ((pixel << 16) & 0xff0000) | (pixel & 0x00ff00) | ((pixel >> 16) & 0x0000ff);
    }
  }

  gdk_draw_rgb_32_image(
    self->object->widget->window,
    self->object->widget->style->fg_gc[GTK_WIDGET_STATE(self->object->widget)],
    0, 0, self->object->widget->allocation.width, self->object->widget->allocation.height,
    GDK_RGB_DITHER_NONE, (guchar*)self->canvas->bufferBGR, self->canvas->pitch
  );
}

void Canvas::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  canvas->bufferRGB = new uint32_t[width * height]();
  canvas->bufferBGR = new uint32_t[width * height]();
  canvas->pitch = width * sizeof(uint32_t);

  object->widget = gtk_drawing_area_new();
  GdkColor color;
  color.pixel = color.red = color.green = color.blue = 0;
  gtk_widget_modify_bg(object->widget, GTK_STATE_NORMAL, &color);
  gtk_widget_set_double_buffered(object->widget, false);
  gtk_widget_add_events(object->widget, GDK_EXPOSURE_MASK);
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "expose_event", G_CALLBACK(Canvas_expose), (gpointer)this);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

uint32_t* Canvas::buffer() {
  return canvas->bufferRGB;
}

void Canvas::redraw() {
  GdkRectangle rect;
  rect.x = 0;
  rect.y = 0;
  rect.width = object->widget->allocation.width;
  rect.height = object->widget->allocation.height;
  gdk_window_invalidate_rect(object->widget->window, &rect, true);
}

Canvas::Canvas() {
  canvas = new Canvas::Data;
  canvas->bufferRGB = 0;
  canvas->bufferBGR = 0;
}

Canvas::~Canvas() {
  if(canvas->bufferRGB) delete[] canvas->bufferRGB;
  if(canvas->bufferBGR) delete[] canvas->bufferBGR;
}
