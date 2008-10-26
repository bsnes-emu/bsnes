void hiro_pcanvas_expose(pCanvas *p) {
  uint32_t *f = p->fbuffer;
  uint32_t *r = p->rbuffer;
  for(unsigned y = p->canvas->allocation.height; y; y--) {
    for(unsigned x = p->canvas->allocation.width; x; x--) {
      uint32_t p = *f++;
      *r++ = ((p << 16) & 0xff0000) + (p & 0x00ff00) + ((p >> 16) & 0x0000ff);
    }
  }

  gdk_draw_rgb_32_image(p->canvas->window,
    p->canvas->style->fg_gc[GTK_WIDGET_STATE(p->canvas)],
    0, 0, p->canvas->allocation.width, p->canvas->allocation.height,
    GDK_RGB_DITHER_NONE, (guchar*)p->rbuffer, p->bpitch);
}

gboolean hiro_pcanvas_button_press(GtkWidget *widget, GdkEventButton *event, pCanvas *p) {
  if(p->self.on_input && event->button < mouse::buttons) {
    p->self.on_input(event_t(event_t::Input, (mouse::button + event->button) + (1 << 16), &p->self));
  }
  return false;  //do not propogate the event to other handlers
}

gboolean hiro_pcanvas_button_release(GtkWidget *widget, GdkEventButton *event, pCanvas *p) {
  if(p->self.on_input && event->button < mouse::buttons) {
    p->self.on_input(event_t(event_t::Input, (mouse::button + event->button) + (0 << 16), &p->self));
  }
  return false;  //do not propogate the event to other handlers
}

void pCanvas::create(unsigned style, unsigned width, unsigned height) {
  canvas = gtk_drawing_area_new();
  resize(width, height);
  GdkColor color;
  color.pixel = color.red = color.green = color.blue = 0;
  gtk_widget_modify_bg(canvas, GTK_STATE_NORMAL, &color);
  gtk_widget_set_double_buffered(canvas, false);
  gtk_widget_add_events(canvas, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
  gtk_widget_show(canvas);
  g_signal_connect_swapped(G_OBJECT(canvas), "expose_event", G_CALLBACK(hiro_pcanvas_expose), (gpointer)this);
  g_signal_connect(G_OBJECT(canvas), "button_press_event", G_CALLBACK(hiro_pcanvas_button_press), (gpointer)this);
  g_signal_connect(G_OBJECT(canvas), "button_release_event", G_CALLBACK(hiro_pcanvas_button_release), (gpointer)this);
}

void pCanvas::redraw() {
  if(!canvas || !canvas->window) return;

  GdkRectangle rect;
  rect.x = 0;
  rect.y = 0;
  rect.width = canvas->allocation.width;
  rect.height = canvas->allocation.height;
  gdk_window_invalidate_rect(canvas->window, &rect, true);
}

uint32_t* pCanvas::buffer() {
  return fbuffer;
}

pCanvas::pCanvas(Canvas &self_) : pFormControl(self_), self(self_) {
  canvas = 0;
  fbuffer = 0;
  rbuffer = 0;
  bpitch = 0;
}

pCanvas::~pCanvas() {
  if(fbuffer) free(fbuffer);
  if(rbuffer) free(rbuffer);
}

/* internal */

void pCanvas::resize(unsigned width, unsigned height) {
  if(fbuffer) free(fbuffer);
  if(rbuffer) free(rbuffer);

  bpitch = width * sizeof(uint32_t);
  fbuffer = (uint32_t*)malloc(bpitch * height);
  rbuffer = (uint32_t*)malloc(bpitch * height);
  memset(fbuffer, 0, bpitch * height);
  memset(rbuffer, 0, bpitch * height);

  pFormControl::resize(width, height);
}

GtkWidget* pCanvas::gtk_handle() {
  return canvas;
}
