#include <gtk/gtk.h>

class VideoGTK : public Video {
private:
GtkWidget *widget;
uint16 *buffer;
uint8  *gdkbuffer;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return gdk_screen_width();  }
  uint screen_height() { return gdk_screen_height(); }

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoGTK(GtkWidget *output_widget);
};
