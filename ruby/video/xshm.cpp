#include <sys/shm.h>
#include <X11/extensions/XShm.h>

namespace ruby {

struct pVideoXShm {
  struct Device {
    Display* display = nullptr;
    int screen;
    int depth;
    Visual* visual = nullptr;
    Window window;

    XShmSegmentInfo shmInfo;
    XImage* image = nullptr;
    uint32_t* buffer = nullptr;
    unsigned width, height;
  } device;

  struct Settings {
    uintptr_t handle;
    unsigned depth = 24;

    uint32_t* buffer = nullptr;
    unsigned width, height;
  } settings;

  struct Color {
    unsigned depth;
    unsigned shift;

    unsigned idepth;
    unsigned ishift;
  } red, green, blue;

  bool cap(const string& name) {
    if(name == Video::Handle) return true;
    if(name == Video::Depth) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return settings.handle;
    if(name == Video::Depth) return settings.depth;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Video::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }
    if(name == Video::Depth) {
      return setDepth(any_cast<unsigned>(value));
    }
    return false;
  }

  bool lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) {
    if(settings.buffer == nullptr || settings.width != width || settings.height != height) {
      if(settings.buffer) delete[] settings.buffer;
      settings.width = width, settings.height = height;
      settings.buffer = new uint32_t[width * height]();
    }

    data = settings.buffer;
    pitch = settings.width * sizeof(uint32_t);
    return true;
  }

  void unlock() {
  }

  void clear() {
    if(settings.buffer == nullptr) return;
    memset(settings.buffer, 0, settings.width * settings.height * sizeof(uint32_t));
    refresh();
  }

  void refresh() {
    if(settings.buffer == nullptr) return;
    size();

    float xRatio = (float)settings.width  / (float)device.width;
    float yRatio = (float)settings.height / (float)device.height;
    float yStep = 0;
    for(unsigned y = 0; y < device.height; y++) {
      uint32_t* sp = settings.buffer + (unsigned)yStep * settings.width;
      uint32_t* dp = device.buffer + y * device.width;
      yStep += yRatio;
      float xStep = 0;
      for(unsigned x = 0; x < device.width; x++) {
        uint32_t color = sp[(unsigned)xStep];
        xStep += xRatio;
        unsigned r = (color >> red.ishift  ) & ((1 << red.idepth  ) - 1);
        unsigned g = (color >> green.ishift) & ((1 << green.idepth) - 1);
        unsigned b = (color >> blue.ishift ) & ((1 << blue.idepth ) - 1);
        *dp++ = image::normalize(r, red.idepth,   red.depth  ) << red.shift
              | image::normalize(g, green.idepth, green.depth) << green.shift
              | image::normalize(b, blue.idepth,  blue.depth ) << blue.shift;
      }
    }

    GC gc = XCreateGC(device.display, device.window, 0, 0);
    XShmPutImage(
      device.display, device.window, gc, device.image,
      0, 0, 0, 0, device.width, device.height, False
    );
    XFreeGC(device.display, gc);
    XFlush(device.display);
  }

  bool init() {
    device.display = XOpenDisplay(0);
    device.screen = DefaultScreen(device.display);

    XWindowAttributes getAttributes;
    XGetWindowAttributes(device.display, (Window)settings.handle, &getAttributes);
    device.depth = getAttributes.depth;
    device.visual = getAttributes.visual;
    unsigned visualID = XVisualIDFromVisual(device.visual);

    XVisualInfo visualTemplate = {0};
    visualTemplate.screen = device.screen;
    visualTemplate.depth = device.depth;
    int visualsMatched = 0;
    XVisualInfo* visualList = XGetVisualInfo(device.display, VisualScreenMask | VisualDepthMask, &visualTemplate, &visualsMatched);
    for(unsigned n = 0; n < visualsMatched; n++) {
      auto& v = visualList[n];
      if(v.visualid == visualID) {
        red.depth   = bit::count(v.red_mask),   red.shift   = bit::first(v.red_mask);
        green.depth = bit::count(v.green_mask), green.shift = bit::first(v.green_mask);
        blue.depth  = bit::count(v.blue_mask),  blue.shift  = bit::first(v.blue_mask);
        break;
      }
    }
    XFree(visualList);
    setDepth(settings.depth);

    XSetWindowAttributes setAttributes = {0};
    setAttributes.border_pixel = 0;
    device.window = XCreateWindow(device.display, (Window)settings.handle,
      0, 0, 256, 256, 0,
      getAttributes.depth, InputOutput, getAttributes.visual,
      CWBorderPixel, &setAttributes
    );
    XSetWindowBackground(device.display, device.window, 0);
    XMapWindow(device.display, device.window);
    XFlush(device.display);

    while(XPending(device.display)) {
      XEvent event;
      XNextEvent(device.display, &event);
    }

    if(size() == false) return false;
    return true;
  }

  void term() {
    free();

    if(device.display) { XCloseDisplay(device.display); device.display = nullptr; }
  }

  ~pVideoXShm() {
    term();
  }

//internal:
  bool setDepth(unsigned depth) {
    if(depth == 24) {
      settings.depth = 24;
      red.idepth   = 8, red.ishift   = 16;
      green.idepth = 8, green.ishift =  8;
      blue.idepth  = 8, blue.ishift  =  0;
      return true;
    }

    if(depth == 30) {
      settings.depth = 30;
      red.idepth   = 10, red.ishift   = 20;
      green.idepth = 10, green.ishift = 10;
      blue.idepth  = 10, blue.ishift  =  0;
      return true;
    }

    return false;
  }

  bool size() {
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(device.display, settings.handle, &windowAttributes);

    if(device.buffer && device.width == windowAttributes.width && device.height == windowAttributes.height) return true;
    device.width = windowAttributes.width, device.height = windowAttributes.height;
    XResizeWindow(device.display, device.window, device.width, device.height);
    free();

    //create
    device.shmInfo.shmid = shmget(IPC_PRIVATE, device.width * device.height * sizeof(uint32_t), IPC_CREAT | 0777);
    if(device.shmInfo.shmid < 0) return false;

    device.shmInfo.shmaddr = (char*)shmat(device.shmInfo.shmid, 0, 0);
    device.shmInfo.readOnly = False;
    XShmAttach(device.display, &device.shmInfo);
    device.buffer = (uint32_t*)device.shmInfo.shmaddr;
    device.image = XShmCreateImage(device.display, device.visual, device.depth,
      ZPixmap, device.shmInfo.shmaddr, &device.shmInfo, device.width, device.height
    );

    return true;
  }

  void free() {
    if(device.buffer == nullptr) return;
    device.buffer = nullptr;
    XShmDetach(device.display, &device.shmInfo);
    XDestroyImage(device.image);
    shmdt(device.shmInfo.shmaddr);
    shmctl(device.shmInfo.shmid, IPC_RMID, 0);
  }
};

DeclareVideo(XShm)

}
