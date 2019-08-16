//XShm driver for Xorg

//Note that on composited displays, the alpha bits will allow translucency underneath the active window
//As this is not a feature of ruby, this driver must always set the alpha bits on clear() and refresh()

//Linear interpolation is only applied horizontally for performance reasons, although Nearest is still much faster

#include <sys/shm.h>
#include <X11/extensions/XShm.h>

struct VideoXShm : VideoDriver {
  VideoXShm& self = *this;
  VideoXShm(Video& super) : VideoDriver(super) { construct(); }
  ~VideoXShm() { destruct(); }

  auto create() -> bool override {
    VideoDriver::exclusive = true;
    return initialize();
  }

  auto driver() -> string override { return "XShm"; }
  auto ready() -> bool override { return _ready; }

  auto hasFullScreen() -> bool override { return true; }
  auto hasMonitor() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setFullScreen(bool fullScreen) -> bool override { return initialize(); }
  auto setMonitor(string monitor) -> bool override { return initialize(); }
  auto setContext(uintptr context) -> bool override { return initialize(); }
  auto setShader(string shader) -> bool override { return true; }

  auto clear() -> void override {
    auto dp = _inputBuffer;
    uint length = _inputWidth * _inputHeight;
    while(length--) *dp++ = 255u << 24;
    output();
  }

  auto size(uint& width, uint& height) -> void override {
    if(self.fullScreen) {
      width = _monitorWidth;
      height = _monitorHeight;
    } else {
      XWindowAttributes parent;
      XGetWindowAttributes(_display, _parent, &parent);
      width = parent.width;
      height = parent.height;
    }
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!_inputBuffer || _inputWidth != width || _inputHeight != height) {
      if(_inputBuffer) delete[] _inputBuffer;
      _inputWidth = width;
      _inputHeight = height;
      _inputBuffer = new uint32_t[width * height + 16];  //+16 is padding for linear interpolation
    }

    data = _inputBuffer;
    pitch = _inputWidth * sizeof(uint32_t);
    return true;
  }

  auto release() -> void override {
  }

  auto output(uint width = 0, uint height = 0) -> void override {
    XWindowAttributes window;
    XGetWindowAttributes(_display, _window, &window);

    XWindowAttributes parent;
    XGetWindowAttributes(_display, _parent, &parent);

    if(window.width != parent.width || window.height != parent.height) {
      _outputWidth = parent.width;
      _outputHeight = parent.height;
      XResizeWindow(_display, _window, _outputWidth, _outputHeight);
      allocate();
    }

    uint viewportX = 0;
    uint viewportY = 0;
    uint viewportWidth = parent.width;
    uint viewportHeight = parent.height;

    if(self.fullScreen) {
      viewportX = _monitorX;
      viewportY = _monitorY;
      viewportWidth = _monitorWidth;
      viewportHeight = _monitorHeight;
    }

    if(!_image || !_inputBuffer || !_outputBuffer) return;

    if(!width) width = viewportWidth;
    if(!height) height = viewportHeight;

    float xratio = (float)_inputWidth / (float)width;
    float yratio = (float)_inputHeight / (float)height;

    int x = ((int)viewportWidth - (int)width) / 2;
    int y = ((int)viewportHeight - (int)height) / 2;

    width = min(width, viewportWidth);
    height = min(height, viewportHeight);

    auto inputBuffer = _inputBuffer;
    auto outputBuffer = _outputBuffer;

    if(x < 0) {
      inputBuffer += abs(x);
      x = 0;
    }

    if(y < 0) {
      inputBuffer += abs(y) * _inputWidth;
      y = 0;
    }

    x += viewportX;
    y += viewportY;

    #pragma omp parallel for
    for(uint y = 0; y < height; y++) {
      float ystep = y * yratio;
      float xstep = 0;

      uint32_t* sp = inputBuffer + (uint)ystep * _inputWidth;
      uint32_t* dp = outputBuffer + y * _outputWidth;

      if(self.shader != "Blur") {
        for(uint x = 0; x < width; x++) {
          *dp++ = 255u << 24 | sp[(uint)xstep];
          xstep += xratio;
        }
      } else {
        for(uint x = 0; x < width; x++) {
          *dp++ = 255u << 24 | interpolate(xstep - (uint)xstep, sp[(uint)xstep], sp[(uint)xstep + 1]);
          xstep += xratio;
        }
      }
    }

    GC gc = XCreateGC(_display, _window, 0, 0);
    XShmPutImage(_display, _window, gc, _image, 0, 0, x, y, width, height, False);
    XFreeGC(_display, gc);
    XFlush(_display);
  }

  auto poll() -> void override {
    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
      if(event.type == Expose) {
        XWindowAttributes attributes{};
        XGetWindowAttributes(_display, _window, &attributes);
        super.doUpdate(attributes.width, attributes.height);
      }
    }
  }

private:
  auto construct() -> void {
    _display = XOpenDisplay(nullptr);
    _screen = DefaultScreen(_display);
    XSetErrorHandler(errorHandler);
  }

  auto destruct() -> void {
    terminate();
    XCloseDisplay(_display);
  }

  auto initialize() -> bool {
    terminate();
    if(!self.fullScreen && !self.context) return false;

    _parent = self.fullScreen ? RootWindow(_display, _screen) : (Window)self.context;
    XWindowAttributes windowAttributes{};
    XGetWindowAttributes(_display, _parent, &windowAttributes);
    //driver only supports 32-bit pixels
    //note that even on 15-bit and 16-bit displays, the window visual's depth should be 32
    if(windowAttributes.depth < 24 || windowAttributes.depth > 32) return free(), false;

    auto monitor = Video::monitor(self.monitor);
    _monitorX = monitor.x;
    _monitorY = monitor.y;
    _monitorWidth = monitor.width;
    _monitorHeight = monitor.height;

    _depth = windowAttributes.depth;
    _visual = windowAttributes.visual;
    _colormap = XCreateColormap(_display, _parent, _visual, AllocNone);
    XSetWindowAttributes attributes{};
    attributes.border_pixel = 0;
    attributes.colormap = _colormap;
    attributes.override_redirect = self.fullScreen;

    _window = XCreateWindow(_display, _parent,
      0, 0, windowAttributes.width, windowAttributes.height,
      0, _depth, InputOutput, _visual,
      CWBorderPixel | CWColormap | CWOverrideRedirect, &attributes
    );

    XSelectInput(_display, _window, ExposureMask);
    XSetWindowBackground(_display, _window, 0);
    XMapWindow(_display, _window);
    XFlush(_display);

    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
    }

    _outputWidth = windowAttributes.width;
    _outputHeight = windowAttributes.height;
    allocate();
    return _ready = (bool)_outputBuffer;
  }

  auto terminate() -> void {
    _ready = false;

    free();

    if(_window) {
      XUnmapWindow(_display, _window);
      _window = 0;
    }

    if(_colormap) {
      XFreeColormap(_display, _colormap);
      _colormap = 0;
    }
  }

  auto allocate() -> void {
    free();

    _shmInfo.shmid = shmget(IPC_PRIVATE, _outputWidth * _outputHeight * sizeof(uint32_t), IPC_CREAT | 0777);
    if(_shmInfo.shmid < 0) return;

    _shmInfo.shmaddr = (char*)shmat(_shmInfo.shmid, 0, 0);
    _shmInfo.readOnly = False;
    XShmAttach(_display, &_shmInfo);
    _outputBuffer = (uint32_t*)_shmInfo.shmaddr;
    _image = XShmCreateImage(_display, _visual, _depth, ZPixmap, _shmInfo.shmaddr, &_shmInfo, _outputWidth, _outputHeight);
  }

  auto free() -> void {
    if(_image) {
      XShmDetach(_display, &_shmInfo);
      XDestroyImage(_image);
      shmdt(_shmInfo.shmaddr);
      shmctl(_shmInfo.shmid, IPC_RMID, 0);
      _image = nullptr;
    }

    _outputBuffer = nullptr;
  }

  alwaysinline auto interpolate(float mu, uint32_t a, uint32_t b) -> uint32_t {
    uint8_t ar = a >> 16, ag = a >> 8, ab = a >> 0;
    uint8_t br = b >> 16, bg = b >> 8, bb = b >> 0;
    uint8_t cr = ar * (1.0 - mu) + br * mu;
    uint8_t cg = ag * (1.0 - mu) + bg * mu;
    uint8_t cb = ab * (1.0 - mu) + bb * mu;
    return cr << 16 | cg << 8 | cb << 0;
  }

  static auto errorHandler(Display* display, XErrorEvent* event) -> int {
    //catch occasional BadAccess errors during window resize events
    //currently, I'm unsure of the cause, but they're certainly not fatal
    return 0;
  }

  bool _ready = false;

  uint32_t* _inputBuffer = nullptr;
  uint _inputWidth = 0;
  uint _inputHeight = 0;

  Display* _display = nullptr;
  uint _monitorX = 0;
  uint _monitorY = 0;
  uint _monitorWidth = 0;
  uint _monitorHeight = 0;
  int _screen = 0;
  int _depth = 0;
  Visual* _visual = nullptr;
  Window _parent = 0;
  Window _window = 0;
  Colormap _colormap = 0;

  XShmSegmentInfo _shmInfo;
  XImage* _image = nullptr;

  uint32_t* _outputBuffer = nullptr;
  uint _outputWidth = 0;
  uint _outputHeight = 0;
};
