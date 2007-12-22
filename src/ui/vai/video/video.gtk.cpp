#include "video.gtk.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class pVideoGTK {
public:
  VideoGTK &self;
  GtkWidget *widget;
  uint16 *buffer;
  uint8 *gdkbuffer;

  struct {
    Window handle;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return false;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return settings.handle;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = param;
      return false;
    }
    return false;
  }

  bool lock(uint16 *&data, uint &pitch) {
    pitch = 1024 * 2;
    return data = buffer;
  }

  void unlock() {
  }

  void refresh(uint r_width, uint r_height) {
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

  void init_no_handle() {
  GdkColor color;
    color.pixel = color.red = color.green = color.blue = 0;
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "video.gtk");
    gtk_window_set_resizable(GTK_WINDOW(window), false);
    gtk_widget_set_size_request(window, 512, 480);
  GtkWidget *container = gtk_fixed_new();
    gtk_widget_set_size_request(container, 512, 480);
    gtk_container_add(GTK_CONTAINER(window), container);
    widget = gtk_drawing_area_new();
    gtk_fixed_put(GTK_FIXED(container), widget, 0, 0);
    gtk_widget_set_size_request(widget, 512, 480);
    gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_bg(container, GTK_STATE_NORMAL, &color);
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
    gtk_widget_show(widget);
    gtk_widget_show(container);
    gtk_widget_show(window);

    if(settings.handle) {
    /* below does not work, sadly ...
      XReparentWindow(
        GDK_WINDOW_XDISPLAY(window->window),
        GDK_WINDOW_XWINDOW(window->window),
        settings.handle, 0, 0);
    */
    }
  }

  void init() {
    buffer    = (uint16*)malloc(1024 * 1024 * 2);
    gdkbuffer = (uint8*) malloc(1024 * 1024 * 3);

    init_no_handle();
  }

  void term() {
    safe_free(buffer);
    safe_free(gdkbuffer);
  }

  pVideoGTK(VideoGTK &self_) : self(self_) {
    settings.handle = 0;
  }
};

bool VideoGTK::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoGTK::get(Setting setting) { return p.get(setting); }
bool VideoGTK::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoGTK::lock(uint16 *&data, uint &pitch) { return p.lock(data, pitch); }
void VideoGTK::unlock() { p.unlock(); }
void VideoGTK::refresh(uint width, uint height) { p.refresh(width, height); }
void VideoGTK::init() { p.init(); }
void VideoGTK::term() { p.term(); }
VideoGTK::VideoGTK() : p(*new pVideoGTK(*this)) {}
VideoGTK::~VideoGTK() { delete &p; }
