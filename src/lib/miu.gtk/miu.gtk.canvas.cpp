void miu_pcanvas_expose(pCanvas *p) {
uint32 *f = p->fbuffer;
uint32 *r = p->rbuffer;
  for(uint y = p->canvas->allocation.height; y; y--) {
    for(uint x = p->canvas->allocation.width; x; x--) {
    uint32 p = *f++;
      *r++ = ((p << 16) & 0xff0000) + (p & 0x00ff00) + ((p >> 16) & 0x0000ff);
    }
  }

  gdk_draw_rgb_32_image(p->canvas->window,
    p->canvas->style->fg_gc[GTK_WIDGET_STATE(p->canvas)],
    0, 0, p->canvas->allocation.width, p->canvas->allocation.height,
    GDK_RGB_DITHER_NONE, (guchar*)p->rbuffer, p->bpitch);
}

void pCanvas::create(uint style, uint width, uint height) {
  canvas = gtk_drawing_area_new();
  resize(width, height);
GdkColor color;
  color.pixel = color.red = color.green = color.blue = 0;
  gtk_widget_modify_bg(canvas, GTK_STATE_NORMAL, &color);
  gtk_widget_show(canvas);

  g_signal_connect_swapped(G_OBJECT(canvas), "expose_event", G_CALLBACK(miu_pcanvas_expose), (gpointer)this);
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

uint32* pCanvas::buffer() {
  return fbuffer;
}

pCanvas::pCanvas(Canvas &self_) : pFormControl(self_), self(self_) {
  canvas = 0;
  fbuffer = 0;
  rbuffer = 0;
  bpitch = 0;
}

pCanvas::~pCanvas() {
  safe_free(fbuffer);
  safe_free(rbuffer);
}

/* internal */

void pCanvas::resize(uint width, uint height) {
  safe_free(fbuffer);
  safe_free(rbuffer);

  bpitch = width * sizeof(uint32);
  fbuffer = (uint32*)malloc(bpitch * height);
  rbuffer = (uint32*)malloc(bpitch * height);
  memset(fbuffer, 0, bpitch * height);
  memset(rbuffer, 0, bpitch * height);

  pFormControl::resize(width, height);
}

GtkWidget* pCanvas::gtk_handle() {
  return canvas;
}
