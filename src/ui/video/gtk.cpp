#include "gtk.h"

bool VideoGTK::lock(uint16 *&data, uint &pitch) {
  pitch = 1024 * 2;
  data  = buffer;
  return data;
}

void VideoGTK::unlock() {}

void VideoGTK::refresh(uint r_width, uint r_height) {
  for(uint y = 0; y < r_height; y++) {
  uint8  *dest = gdkbuffer + y * 1024 * 3;
  uint16 *src  = buffer + y * 1024;
    for(uint x = 0; x < r_width; x++) {
    uint32 p = *src++;
    uint32 pix = ((p & 0xf800) << 8) | ((p & 0x07e0) << 5) | ((p & 0x001f) << 3);
      *dest++ = pix >> 16;
      *dest++ = pix >> 8;
      *dest++ = pix;
    }
  }

  gdk_draw_rgb_image(GDK_DRAWABLE(widget->window),
    widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
    0, 0, r_width, r_height,
    GDK_RGB_DITHER_NONE, gdkbuffer, 1024 * 3);
}

void VideoGTK::init() {
  buffer    = (uint16*)malloc(1024 * 1024 * 2);
  gdkbuffer = (uint8*) malloc(1024 * 1024 * 3);
}

void VideoGTK::term() {
  safe_free(buffer);
  safe_free(gdkbuffer);
}

VideoGTK::VideoGTK(GtkWidget *output_widget) {
  widget = output_widget;
}
