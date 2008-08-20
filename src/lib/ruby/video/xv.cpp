#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>

extern "C" XvImage* XvShmCreateImage(Display*, XvPortID, int, char*, int, int, XShmSegmentInfo*);

#include <ruby/ruby.h>

namespace ruby {

#include "xv.h"

class pVideoXv {
public:
  VideoXv &self;
  Display *display;
  GC gc;
  int screen, xv_port, xv_depth, xv_visualid;
  XvImage *xvimage;
  XShmSegmentInfo shminfo;
  uint32_t *buffer;

  bool use_child_window;
  Window xwindow;
  Colormap colormap;

  uint8_t *ytable, *utable, *vtable;

  struct {
    Window handle;
    bool synchronize;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    if(setting == Video::Synchronize) {
      return XInternAtom(XOpenDisplay(0), "XV_SYNC_TO_VBLANK", true) != None;
    }
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return settings.handle;
    if(setting == Video::Synchronize) return settings.synchronize;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = param;
      return true;
    }

    if(setting == Video::Synchronize) {
      Display *display = XOpenDisplay(0);
      Atom atom = XInternAtom(display, "XV_SYNC_TO_VBLANK", true);
      if(atom != None) {
        settings.synchronize = param;
        XvSetPortAttribute(display, xv_port, atom, settings.synchronize);
        return true;
      }
      return false;
    }

    return false;
  }

  bool lock(uint32_t *&data, unsigned &pitch) {
    pitch = 1024 * 4;
    return data = buffer;
  }

  void unlock() {
  }

  void clear() {
    memset(buffer, 0, 1024 * 1024 * sizeof(uint32_t));
    //clear twice in case video is double buffered ...
    refresh(1024, 1024);
    refresh(1024, 1024);
  }

  void refresh(unsigned width, unsigned height) {
    XWindowAttributes target;
    XGetWindowAttributes(display, xwindow, &target);

    if(use_child_window) {
      //we must ensure that the child window is the same size as the parent window.
      //unfortunately, we cannot hook the parent window resize event notification,
      //as we did not create the parent window, nor have any knowledge of the toolkit used.
      //therefore, inelegant as it may be, we query each window size and resize as needed.
      XWindowAttributes parent;
      XGetWindowAttributes(display, settings.handle, &parent);
      if(target.width != parent.width || target.height != parent.height) {
        XResizeWindow(display, xwindow, parent.width, parent.height);
      }

      //update target width and height attributes
      XGetWindowAttributes(display, xwindow, &target);
    }

    uint32_t *input  = (uint32_t*)buffer;
    uint16_t *output = (uint16_t*)xvimage->data;
    for(unsigned y = 0; y < height; y++) {
      for(unsigned x = 0; x < width >> 1; x++) {
        uint32_t p0 = *input++;
        uint32_t p1 = *input++;
        p0 = ((p0 >> 8) & 0xf800) + ((p0 >> 5) & 0x07e0) + ((p0 >> 3) & 0x001f);
        p1 = ((p1 >> 8) & 0xf800) + ((p1 >> 5) & 0x07e0) + ((p1 >> 3) & 0x001f);

        uint8_t u = (utable[p0] + utable[p1]) >> 1;
        uint8_t v = (vtable[p0] + vtable[p1]) >> 1;

        *output++ = (u << 8) | ytable[p0];
        *output++ = (v << 8) | ytable[p1];
      }
      input  += 1024 - width;
      output += 1024 - width;
    }

    XvShmPutImage(display, xv_port, xwindow, gc, xvimage,
      0, 0, width, height,
      0, 0, target.width, target.height,
      true);
  }

  bool init() {
    display = XOpenDisplay(0);
    screen  = DefaultScreen(display);

    //XShm is required for rendering
    if(!XShmQueryExtension(display)) {
      fprintf(stderr, "VideoXv: XShm extension not found.\n");
      return false;
    }

    //find an appropriate port, if possible
    xv_port = -1;
    XvAdaptorInfo *adaptor_info;
    unsigned adaptor_count;
    XvQueryAdaptors(display, DefaultRootWindow(display), &adaptor_count, &adaptor_info);
    for(unsigned i = 0; i < adaptor_count; i++) {
      //find adaptor that supports both input (memory->drawable) and image (drawable->screen) masks
      if(adaptor_info[i].num_formats < 1) continue;
      if(!(adaptor_info[i].type & XvInputMask)) continue;
      if(!(adaptor_info[i].type & XvImageMask)) continue;

      xv_port     = adaptor_info[i].base_id;
      xv_depth    = adaptor_info[i].formats->depth;
      xv_visualid = adaptor_info[i].formats->visual_id;
      break;
    }
    XvFreeAdaptorInfo(adaptor_info);
    if(xv_port == -1) {
      fprintf(stderr, "VideoXv: failed to find valid XvPort.\n");
      return false;
    }

    XWindowAttributes window_attributes;
    XGetWindowAttributes(display, settings.handle, &window_attributes);

    if(xv_depth == window_attributes.depth) {
      //Xv port is depth-compatible with target output window
      use_child_window = false;
      xwindow = settings.handle;
    } else {
      //Xv port is not depth-compatible with target output window
      //this is often the case when a 32bpp composited window is used with a 24bpp-only Xv adaptor
      //the only way to render to target is to create a child window with the Xv ports' depth
      use_child_window = true;
      XVisualInfo visualtemplate;
      visualtemplate.visualid = xv_visualid;
      visualtemplate.screen   = screen;
      visualtemplate.depth    = xv_depth;
      visualtemplate.visual   = 0;
      int visualmatches       = 0;
      XVisualInfo *visualinfo = XGetVisualInfo(display, VisualIDMask | VisualScreenMask | VisualDepthMask, &visualtemplate, &visualmatches);
      if(visualmatches < 1 || !visualinfo->visual) {
        if(visualinfo) XFree(visualinfo);
        fprintf(stderr, "VideoXv: unable to find Xv-compatible visual.\n");
        return false;
      }

      colormap = XCreateColormap(display, settings.handle, visualinfo->visual, AllocNone);
      XSetWindowAttributes attributes;
      attributes.colormap = colormap;
      attributes.border_pixel = 0;
      attributes.event_mask = StructureNotifyMask;
      xwindow = XCreateWindow(display, /* parent = */ settings.handle,
        /* x = */ 0, /* y = */ 0, window_attributes.width, window_attributes.height,
        /* border_width = */ 0, xv_depth, InputOutput, visualinfo->visual,
        CWColormap | CWBorderPixel | CWEventMask, &attributes);
      XFree(visualinfo);
      XSetWindowBackground(display, xwindow, /* color = */ 0);
      XMapWindow(display, xwindow);
    }

    gc = XCreateGC(display, xwindow, 0, 0);

    //set colorkey to auto paint, so that Xv video output is always visible
    Atom atom = XInternAtom(display, "XV_AUTOPAINT_COLORKEY", true);
    if(atom != None) XvSetPortAttribute(display, xv_port, atom, 1);

    //0x32595559 = 16-bit Y8U8,Y8V8 (YUY2)
    xvimage = XvShmCreateImage(display, xv_port, 0x32595559, 0, 1024, 1024, &shminfo);
    if(!xvimage) {
      fprintf(stderr, "VideoXv: XShmCreateImage failed.\n");
      return false;
    }

    shminfo.shmid    = shmget(IPC_PRIVATE, xvimage->data_size, IPC_CREAT | 0777);
    shminfo.shmaddr  = xvimage->data = (char*)shmat(shminfo.shmid, 0, 0);
    shminfo.readOnly = false;
    if(!XShmAttach(display, &shminfo)) {
      fprintf(stderr, "VideoXv: XShmAttach failed.\n");
      return false;
    }

    buffer = new uint32_t[1024 * 1024];
    init_yuv_tables();
    clear();
    return true;
  }

  void term() {
    XShmDetach(display, &shminfo);

    if(use_child_window) {
      if(xwindow) {
        XUnmapWindow(display, xwindow);
        xwindow = 0;
      }

      if(colormap) {
        XFreeColormap(display, colormap);
        colormap = 0;
      }
    }

    if(buffer) { delete[] buffer; buffer = 0; }
    if(ytable) { delete[] ytable; ytable = 0; }
    if(utable) { delete[] utable; utable = 0; }
    if(vtable) { delete[] vtable; vtable = 0; }
  }

  void init_yuv_tables() {
    ytable = new uint8_t[65536];
    utable = new uint8_t[65536];
    vtable = new uint8_t[65536];

    for(unsigned i = 0; i < 65536; i++) {
      //extract RGB565 color data from i
      uint8_t r = (i >> 11) & 31, g = (i >> 5) & 63, b = (i) & 31;
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
    use_child_window = false;
    xwindow = 0;
    colormap = 0;

    ytable = 0;
    utable = 0;
    vtable = 0;

    settings.handle = 0;
    settings.synchronize = false;
  }
};

bool VideoXv::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoXv::get(Setting setting) { return p.get(setting); }
bool VideoXv::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoXv::lock(uint32_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoXv::unlock() { p.unlock(); }
void VideoXv::clear() { p.clear(); }
void VideoXv::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoXv::init() { return p.init(); }
void VideoXv::term() { p.term(); }
VideoXv::VideoXv() : p(*new pVideoXv(*this)) {}
VideoXv::~VideoXv() { delete &p; }

} //namespace ruby
