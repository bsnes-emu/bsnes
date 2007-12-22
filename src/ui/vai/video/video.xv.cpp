#include "video.xv.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>

extern "C" XvImage* XvShmCreateImage(Display*, XvPortID, int, char*, int, int, XShmSegmentInfo*);

class pVideoXv {
public:
  VideoXv &self;
  uint16 *buffer;
  XvImage *xvimage;
  GC gc;
  Display *display;
  int screen, xv_port;
  XShmSegmentInfo shminfo;
  uint8 *ytable, *utable, *vtable;

  struct {
    Window handle;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return settings.handle;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = param;
      return true;
    }
    return false;
  }

  bool lock(uint16 *&data, uint &pitch) {
    pitch = 1024 * 2;
    data  = buffer;
    return data;
  }

  void unlock() {
  }

  void clear_video() {
    memset(buffer, 0, 1024 * 1024 * sizeof(uint16));
  //clear twice in case video is double buffered ...
    refresh(1024, 1024);
    refresh(1024, 1024);
  }

  void refresh(uint r_width, uint r_height) {
  Window dw;
  int d0, d1;
  uint d2, d3;
  uint width, height;
    XGetGeometry(display, settings.handle, &dw, &d0, &d1, &width, &height, &d2, &d3);

  uint16 *input  = (uint16*)buffer;
  uint16 *output = (uint16*)xvimage->data;
    for(int y = 0; y < r_height; y++) {
      for(int x = 0; x < r_width >> 1; x++) {
      uint16 p0 = *input++;
      uint16 p1 = *input++;

      uint8 u = (utable[p0] + utable[p1]) >> 1;
      uint8 v = (vtable[p0] + vtable[p1]) >> 1;

        *output++ = (u << 8) | ytable[p0];
        *output++ = (v << 8) | ytable[p1];
      }
      input  += 1024 - r_width;
      output += 1024 - r_width;
    }

    XvShmPutImage(display, xv_port, settings.handle, gc, xvimage,
      0, 0, r_width, r_height,
      0, 0, width, height,
      true);
  }

  void init() {
    buffer  = (uint16*)malloc(1024 * 1024 * sizeof(uint16));
    display = XOpenDisplay(0);
    screen  = DefaultScreen(display);
    gc      = XCreateGC(display, settings.handle, 0, 0);

  XVisualInfo visual_info;
           if(XMatchVisualInfo(display, screen, 24, TrueColor,   &visual_info)) {
    } else if(XMatchVisualInfo(display, screen, 16, TrueColor,   &visual_info)) {
    } else if(XMatchVisualInfo(display, screen, 15, TrueColor,   &visual_info)) {
    } else if(XMatchVisualInfo(display, screen,  8, PseudoColor, &visual_info)) {
    } else if(XMatchVisualInfo(display, screen,  8, GrayScale,   &visual_info)) {
    } else if(XMatchVisualInfo(display, screen,  8, StaticGray,  &visual_info)) {
    } else if(XMatchVisualInfo(display, screen,  1, StaticGray,  &visual_info)) {
    } else {
      printf("VideoXv: unable to find suitable video display.\n");
    }

    if(!XShmQueryExtension(display)) printf("VideoXv: XShm extension not found.\n");

    xv_port = -1;
  XvAdaptorInfo *adaptor_info;
  uint adaptor_count;
    XvQueryAdaptors(display, DefaultRootWindow(display), &adaptor_count, &adaptor_info);
    for(uint i = 0; i < adaptor_count; i++) {
    //find adaptor that supports both input (memory->drawable) and image (drawable->screen) masks
      if(adaptor_info[i].type & XvInputMask && adaptor_info[i].type & XvImageMask) {
        xv_port = adaptor_info[i].base_id;
        break;
      }
    }
    XvFreeAdaptorInfo(adaptor_info);
    if(xv_port == -1) printf("VideoXv: failed to find valid XvPort.\n");

  //set colorkey to auto paint, so that Xv video output is always visible
  const Atom atom = XInternAtom(display, "XV_AUTOPAINT_COLORKEY", true);
    if(atom != None) XvSetPortAttribute(display, xv_port, atom, 1);

  //0x00000003 = 32-bit X8R8G8B8  [xRGB] (few drivers support this mode)
  //0x32595559 = 16-bit Y8U8,Y8V8 [YUY2] (most drivers support this mode)
    xvimage = XvShmCreateImage(display, xv_port, 0x32595559, 0, 1024, 1024, &shminfo);
    if(!xvimage) printf("VideoXv: XShmCreateImage failed.\n");
    shminfo.shmid    = shmget(IPC_PRIVATE, xvimage->data_size, IPC_CREAT | 0777);
    shminfo.shmaddr  = xvimage->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = false;
    if(!XShmAttach(display, &shminfo)) printf("VideoXv: XShmAttach failed.\n");

    init_yuv_tables();
    clear_video();
  }

  void term() {
    XShmDetach(display, &shminfo);
    free(ytable);
    free(utable);
    free(vtable);
  }

  void init_yuv_tables() {
    ytable = (uint8*)malloc(65536);
    utable = (uint8*)malloc(65536);
    vtable = (uint8*)malloc(65536);

    for(int i = 0; i < 65536; i++) {
    //extract RGB565 color data from i
    uint8 r = (i >> 11) & 31, g = (i >> 5) & 63, b = (i) & 31;
      r = (r << 3) | (r >> 2); //R5->R8
      g = (g << 2) | (g >> 4); //G6->G8
      b = (b << 3) | (b >> 2); //B5->B8

    //RGB->YUV conversion
    int y = int( +(double(r) * 0.257) + (double(g) * 0.504) + (double(b) * 0.098) +  16.0 );
    int u = int( -(double(r) * 0.148) - (double(g) * 0.291) + (double(b) * 0.439) + 128.0 );
    int v = int( +(double(r) * 0.439) - (double(g) * 0.368) - (double(b) * 0.071) + 128.0 );

    //RGB->YCbCr conversion
    //double lr = 0.2126, lb = 0.0722, lg = (1.0 - lr - lb);
    //int y = int( double(r) * lr + double(g) * lg + double(b) * lb );
    //int u = int( (double(b) - y) / (2.0 - 2.0 * lb) + 128.0 );
    //int v = int( (double(r) - y) / (2.0 - 2.0 * lr) + 128.0 );

      ytable[i] = y < 0 ? 0 : y > 255 ? 255 : y;
      utable[i] = u < 0 ? 0 : u > 255 ? 255 : u;
      vtable[i] = v < 0 ? 0 : v > 255 ? 255 : v;
    }
  }

  pVideoXv(VideoXv &self_) : self(self_) {
    settings.handle = 0;
  }
};

bool VideoXv::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoXv::get(Setting setting) { return p.get(setting); }
bool VideoXv::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoXv::lock(uint16 *&data, uint &pitch) { return p.lock(data, pitch); }
void VideoXv::unlock() { p.unlock(); }
void VideoXv::clear_video() { p.clear_video(); }
void VideoXv::refresh(uint width, uint height) { p.refresh(width, height); }
void VideoXv::init() { p.init(); }
void VideoXv::term() { p.term(); }
VideoXv::VideoXv() : p(*new pVideoXv(*this)) {}
VideoXv::~VideoXv() { delete &p; }
