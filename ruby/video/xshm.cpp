//XShm driver for Xorg

//Note that on composited displays, the alpha bits will allow translucency underneath the active window
//As this is not a feature of ruby, this driver must always set the alpha bits on clear() and refresh()

//Linear interpolation is only applied horizontally for performance reasons, although Nearest is still much faster

#include <sys/shm.h>
#include <X11/extensions/XShm.h>

struct VideoXShm : Video {
  ~VideoXShm() { term(); }

  struct Device {
    Display* display = nullptr;
    signed screen = 0;
    signed depth = 0;
    Visual* visual = nullptr;
    Window window = 0;

    XShmSegmentInfo shmInfo;
    XImage* image = nullptr;
    uint32_t* buffer = nullptr;
    unsigned width = 0;
    unsigned height = 0;
  } device;

  struct Settings {
    uintptr_t handle = 0;
    unsigned filter = Video::FilterLinear;

    uint32_t* buffer = nullptr;
    unsigned width = 0;
    unsigned height = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Filter) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return settings.handle;
    if(name == Video::Filter) return settings.filter;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = value.get<uintptr_t>();
      return true;
    }
    if(name == Video::Filter && value.is<unsigned>()) {
      settings.filter = value.get<unsigned>();
      return true;
    }
    return false;
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    if(settings.buffer == nullptr || settings.width != width || settings.height != height) {
      if(settings.buffer) delete[] settings.buffer;
      settings.width = width, settings.height = height;
      settings.buffer = new uint32_t[width * height + 16];  //+16 is padding for linear interpolation
    }

    data = settings.buffer;
    pitch = settings.width * sizeof(uint32_t);
    return true;
  }

  auto unlock() -> void {
  }

  auto clear() -> void {
    if(settings.buffer == nullptr) return;
    uint32_t* dp = settings.buffer;
    unsigned length = settings.width * settings.height;
    while(length--) *dp++ = 255u << 24;
    refresh();
  }

  auto refresh() -> void {
    if(settings.buffer == nullptr) return;
    size();

    float xratio = (float)settings.width  / (float)device.width;
    float yratio = (float)settings.height / (float)device.height;

    #pragma omp parallel for
    for(unsigned y = 0; y < device.height; y++) {
      float ystep = y * yratio;
      float xstep = 0;

      uint32_t* sp = settings.buffer + (unsigned)ystep * settings.width;
      uint32_t* dp = device.buffer + y * device.width;

      if(settings.filter == Video::FilterNearest) {
        for(unsigned x = 0; x < device.width; x++) {
          *dp++ = 255u << 24 | sp[(unsigned)xstep];
          xstep += xratio;
        }
      } else {  //settings.filter == Video::FilterLinear
        for(unsigned x = 0; x < device.width; x++) {
          *dp++ = 255u << 24 | interpolate(xstep - (unsigned)xstep, sp[(unsigned)xstep], sp[(unsigned)xstep + 1]);
          xstep += xratio;
        }
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

  auto init() -> bool {
    device.display = XOpenDisplay(0);
    device.screen = DefaultScreen(device.display);

    XWindowAttributes getAttributes;
    XGetWindowAttributes(device.display, (Window)settings.handle, &getAttributes);
    device.depth = getAttributes.depth;
    device.visual = getAttributes.visual;
    //driver only supports 32-bit pixels
    //note that even on 15-bit and 16-bit displays, the window visual's depth should be 32
    if(device.depth < 24 || device.depth > 32) {
      free();
      return false;
    }

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

  auto term() -> void {
    free();
    if(device.display) {
      XCloseDisplay(device.display);
      device.display = nullptr;
    }
  }

private:
  auto size() -> bool {
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(device.display, settings.handle, &windowAttributes);

    if(device.buffer && device.width == windowAttributes.width && device.height == windowAttributes.height) return true;
    device.width = windowAttributes.width, device.height = windowAttributes.height;
    XResizeWindow(device.display, device.window, device.width, device.height);
    free();

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

  auto free() -> void {
    if(device.buffer == nullptr) return;
    device.buffer = nullptr;
    XShmDetach(device.display, &device.shmInfo);
    XDestroyImage(device.image);
    shmdt(device.shmInfo.shmaddr);
    shmctl(device.shmInfo.shmid, IPC_RMID, 0);
  }

  alwaysinline auto interpolate(float mu, uint32_t a, uint32_t b) -> uint32_t {
    uint8_t ar = a >> 16, ag = a >> 8, ab = a >> 0;
    uint8_t br = b >> 16, bg = b >> 8, bb = b >> 0;
    uint8_t cr = ar * (1.0 - mu) + br * mu;
    uint8_t cg = ag * (1.0 - mu) + bg * mu;
    uint8_t cb = ab * (1.0 - mu) + bb * mu;
    return cr << 16 | cg << 8 | cb << 0;
  }
};
