//WARNING: GTK video renderer is currently broken
//Renderer is displaying incorrect colors

uint16 *VideoGTK::lock(uint &pitch) {
  pitch = 512;
  return (uint16*)buffer;
}

void VideoGTK::unlock() {}

void VideoGTK::redraw() {
uint8  *dest = gdkbuffer;
uint16 *src  = buffer;
uint    z    = 256 * 224;
  while(z--) {
  uint32 p = *src++;
    *(uint32*)dest = ((p & 0xf800) << 8) | ((p & 0x07e0) << 5) | ((p & 0x001f) << 3);
    dest += 3;
  }
  gdk_draw_rgb_image(GDK_DRAWABLE(widget->window), gc, 0, 0, 256, 224, GDK_RGB_DITHER_NONE, gdkbuffer, 768);
}

void VideoGTK::update() { redraw(); }

void VideoGTK::init() {
  buffer    = (uint16*)malloc(256 * 224 * 2);
  gdkbuffer = (uint8*) malloc(256 * 224 * 3);
}

void VideoGTK::term() {
  SafeFree(buffer);
  SafeFree(gdkbuffer);
}

VideoGTK::VideoGTK(GtkWidget *output_widget) {
  widget = output_widget;
  gc = gdk_gc_new(GDK_DRAWABLE(output_widget->window));
}
