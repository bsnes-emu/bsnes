bool VideoGTK::lock(uint16 *&data, uint &pitch) {
  pitch = 512;
  data  = buffer;
  return data;
}

void VideoGTK::unlock() {}

void VideoGTK::redraw() {
uint8  *dest = gdkbuffer;
uint16 *src  = buffer;
uint    z    = 256 * 224;
  while(z--) {
  uint32 p = *src++;
  uint32 pix = ((p & 0xf800) << 8) | ((p & 0x07e0) << 5) | ((p & 0x001f) << 3);
    *dest++ = pix >> 16;
    *dest++ = pix >> 8;
    *dest++ = pix;
  }
  gdk_draw_rgb_image(GDK_DRAWABLE(window), gc, 0, 0, 256, 224, GDK_RGB_DITHER_NONE, gdkbuffer, 768);
}

void VideoGTK::update() { redraw(); }

void VideoGTK::init() {
  buffer    = (uint16*)malloc(256 * 224 * 2);
  gdkbuffer = (uint8*) malloc(256 * 224 * 3);
}

void VideoGTK::term() {
  safe_free(buffer);
  safe_free(gdkbuffer);
}

VideoGTK::VideoGTK(unsigned long output_window) {
  window = output_window;
  gc = gdk_gc_new(GDK_DRAWABLE(window));
}
