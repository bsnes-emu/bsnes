#include <sys/shm.h>
#include <X11/extensions/XShm.h>

namespace ruby {

struct pVideoXShm {
  struct {
    Display *display;
    int screen;
    Visual *visual;
    int depth;
    Window window;

    XShmSegmentInfo shmInfo;
    XImage *image;
    uint32_t *buffer;
    unsigned width, height;
  } device;

  struct {
    uintptr_t handle;

    uint32_t *buffer;
    unsigned width, height;
  } settings;

  bool cap(const string &name) {
    if(name == Video::Handle) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return settings.handle;
  }

  bool set(const string& name, const any& value) {
    if(name == Video::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }
    return false;
  }

  bool lock(uint32_t *&data, unsigned &pitch, unsigned width, unsigned height) {
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

    float xRatio = (float)settings.width / (float)device.width;
    float yRatio = (float)settings.height / (float)device.height;
    float yStep = 0;
    for(unsigned y = 0; y < device.height; y++) {
      uint32_t *sp = settings.buffer + (unsigned)yStep * settings.width;
      uint32_t *dp = device.buffer + y * device.width;
      yStep += yRatio;
      float xStep = 0;
      for(unsigned x = 0; x < device.width; x++) {
        uint32_t color = sp[(unsigned)xStep];
        xStep += xRatio;
        *dp++ = ((color >> 20) & 0x000003ff) | ((color) & 0x000ffc00) | ((color << 20) & 0x3ff00000);
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
    device.visual = DefaultVisual(device.display, device.screen);
    device.depth = DefaultDepth(device.display, device.screen);

    XSetWindowAttributes attributes;
    attributes.border_pixel = 0;
    device.window = XCreateWindow(device.display, (Window)settings.handle,
      0, 0, 256, 256,
      0, device.depth, InputOutput, device.visual,
      CWBorderPixel, &attributes
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
  }

  pVideoXShm() {
    device.buffer = nullptr;

    settings.buffer = nullptr;
  }

  ~pVideoXShm() {
    term();
  }

//internal:
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
