#include <gtk/gtk.h>

class VideoGTK : public Video {
private:
GdkGC  *gc;
uint16 *buffer;
uint8  *gdkbuffer;
unsigned long window;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return gdk_screen_width();  }
  uint screen_height() { return gdk_screen_height(); }

  void redraw();
  void update();
  void init();
  void term();

  VideoGTK(unsigned long output_window);
};
