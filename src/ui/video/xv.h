#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>

extern "C" XvImage* XvShmCreateImage(Display*, XvPortID, int, char*, int, int, XShmSegmentInfo*);

class VideoXv : public Video {
private:
unsigned long window;
uint16 *buffer;
XvImage *xvimage;
GC gc;
Display *display;
int screen, xv_port;
XShmSegmentInfo shminfo;

uint8 *ytable, *utable, *vtable;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return gdk_screen_width();  }
  uint screen_height() { return gdk_screen_height(); }

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoXv(unsigned long handle);
  ~VideoXv();

protected:
  void init_yuv_tables();
};
