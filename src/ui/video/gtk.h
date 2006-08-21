#include <gtk/gtk.h>

class VideoGTK : public Video {
private:
GtkWidget *widget;
GdkGC     *gc;
uint16    *buffer;
uint8     *gdkbuffer;

public:
  uint16 *lock(uint &pitch);
  void    unlock();

  uint    screen_width()  { return 1152; }
  uint    screen_height() { return  864; }

  void    redraw();
  void    update();
  void    init();
  void    term();

  VideoGTK(GtkWidget *output_widget);
};
