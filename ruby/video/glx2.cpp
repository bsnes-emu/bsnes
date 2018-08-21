//Xorg/GLX OpenGL 2.0 driver

//note: this is a fallback driver for use when OpenGL 3.2 is not available.
//see glx.cpp for comments on how this driver operates (they are very similar.)

#if defined(DISPLAY_XORG)
  #include <GL/gl.h>
  #include <GL/glx.h>
  #ifndef glGetProcAddress
    #define glGetProcAddress(name) (*glXGetProcAddress)((const GLubyte*)(name))
  #endif
#elif defined(DISPLAY_QUARTZ)
  #include <OpenGL/gl3.h>
#elif defined(DISPLAY_WINDOWS)
  #include <GL/gl.h>
  #include <GL/glext.h>
  #ifndef glGetProcAddress
    #define glGetProcAddress(name) wglGetProcAddress(name)
  #endif
#else
  #error "ruby::OpenGL2: unsupported platform"
#endif

struct VideoGLX2 : VideoDriver {
  VideoGLX2& self = *this;
  VideoGLX2(Video& super) : VideoDriver(super) {}
  ~VideoGLX2() { terminate(); }

  auto create() -> bool {
    super.setFormat("RGB24");
    return initialize();
  }

  auto driver() -> string override { return "OpenGL 2.0"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasFormats() -> vector<string> override { return {"RGB24"}; }
  auto hasShader() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(glXSwapInterval) glXSwapInterval(blocking);
    return true;
  }

  auto setFormat(string format) -> bool override {
    if(format == "RGB24") {
      _glFormat = GL_UNSIGNED_INT_8_8_8_8_REV;
      return initialize();
    }

    if(format == "RGB30") {
      _glFormat = GL_UNSIGNED_INT_2_10_10_10_REV;
      return initialize();
    }

    return false;
  }

  auto setShader(string shader) -> bool override {
    return true;
  }

  auto configure(uint width, uint height, double inputFrequency, double outputFrequency) -> bool override {
    XResizeWindow(_display, _window, width, height);
    return true;
  }

  auto clear() -> void override {
    memory::fill<uint32_t>(_glBuffer, _glWidth * _glHeight);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    if(_isDoubleBuffered) glXSwapBuffers(_display, _glXWindow);
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(width != _width || height != _height) resize(width, height);
    pitch = _glWidth * sizeof(uint32_t);
    return data = _glBuffer;
  }

  auto release() -> void override {
  }

  auto output() -> void override {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, self.shader == "Blur" ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, self.shader == "Blur" ? GL_LINEAR : GL_NEAREST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, self.width, 0, self.height, -1.0, 1.0);
    glViewport(0, 0, self.width, self.height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _glWidth);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_BGRA, _glFormat, _glBuffer);

    double w = (double)_width / (double)_glWidth;
    double h = (double)_height / (double)_glHeight;
    int u = self.width;
    int v = self.height;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0, 0); glVertex3i(0, v, 0);
    glTexCoord2f(w, 0); glVertex3i(u, v, 0);
    glTexCoord2f(0, h); glVertex3i(0, 0, 0);
    glTexCoord2f(w, h); glVertex3i(u, 0, 0);
    glEnd();
    glFlush();

    if(_isDoubleBuffered) glXSwapBuffers(_display, _glXWindow);
    if(self.flush) glFinish();
  }

  auto poll() -> void override {
    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
      if(event.type == Expose) {
        XWindowAttributes attributes;
        XGetWindowAttributes(_display, _window, &attributes);
        super.doUpdate(attributes.width, attributes.height);
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!self.context) return false;

    _display = XOpenDisplay(nullptr);
    _screen = DefaultScreen(_display);

    int versionMajor = 0, versionMinor = 0;
    glXQueryVersion(_display, &versionMajor, &versionMinor);
    if(versionMajor < 1 || (versionMajor == 1 && versionMinor < 2)) return false;

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(_display, (Window)self.context, &windowAttributes);

    int redDepth   = self.format == "RGB30" ? 10 : 8;
    int greenDepth = self.format == "RGB30" ? 10 : 8;
    int blueDepth  = self.format == "RGB30" ? 10 : 8;

    int attributeList[] = {
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DOUBLEBUFFER, True,
      GLX_RED_SIZE, redDepth,
      GLX_GREEN_SIZE, greenDepth,
      GLX_BLUE_SIZE, blueDepth,
      None
    };

    int fbCount = 0;
    auto fbConfig = glXChooseFBConfig(_display, _screen, attributeList, &fbCount);
    if(fbCount == 0) return false;

    auto vi = glXGetVisualFromFBConfig(_display, fbConfig[0]);
    _colormap = XCreateColormap(_display, RootWindow(_display, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes attributes = {};
    attributes.colormap = _colormap;
    attributes.border_pixel = 0;
    _window = XCreateWindow(_display, (Window)self.context, 0, 0, windowAttributes.width, windowAttributes.height,
      0, vi->depth, InputOutput, vi->visual, CWColormap | CWBorderPixel, &attributes);
    XSelectInput(_display, _window, ExposureMask);
    XSetWindowBackground(_display, _window, 0);
    XMapWindow(_display, _window);
    XFlush(_display);

    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
    }

    _glXContext = glXCreateContext(_display, vi, 0, GL_TRUE);
    glXMakeCurrent(_display, _glXWindow = _window, _glXContext);

    if(!glXSwapInterval) glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalMESA");
    if(!glXSwapInterval) glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalSGI");

    if(glXSwapInterval) glXSwapInterval(self.blocking);

    int value = 0;
    glXGetConfig(_display, vi, GLX_DOUBLEBUFFER, &value);
    _isDoubleBuffered = value;
    _isDirect = glXIsDirect(_display, _glXContext);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_DITHER);
    glEnable(GL_TEXTURE_2D);

    resize(256, 256);
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;

    if(_glTexture) {
      glDeleteTextures(1, &_glTexture);
      _glTexture = 0;
    }

    if(_glBuffer) {
      delete[] _glBuffer;
      _glBuffer = nullptr;
    }

    _glWidth = 0;
    _glHeight = 0;

    if(_glXContext) {
      glXDestroyContext(_display, _glXContext);
      _glXContext = nullptr;
    }

    if(_window) {
      XUnmapWindow(_display, _window);
      _window = 0;
    }

    if(_colormap) {
      XFreeColormap(_display, _colormap);
      _colormap = 0;
    }

    if(_display) {
      XCloseDisplay(_display);
      _display = nullptr;
    }
  }

  auto resize(uint width, uint height) -> void {
    _width = width;
    _height = height;

    if(_glTexture == 0) glGenTextures(1, &_glTexture);
    _glWidth = max(_glWidth, width);
    _glHeight = max(_glHeight, height);
    delete[] _glBuffer;
    _glBuffer = new uint32_t[_glWidth * _glHeight]();

    glBindTexture(GL_TEXTURE_2D, _glTexture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _glWidth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _glWidth, _glHeight, 0, GL_BGRA, _glFormat, _glBuffer);
  }

  bool _ready = false;
  bool blur = false;

  Display* _display = nullptr;
  int _screen = 0;
  Window _window = 0;
  Colormap _colormap = 0;
  GLXContext _glXContext = nullptr;
  GLXWindow _glXWindow = 0;

  bool _isDoubleBuffered = false;
  bool _isDirect = false;

  uint _width = 256;
  uint _height = 256;

  GLuint _glTexture = 0;
  uint32_t* _glBuffer = nullptr;
  uint _glWidth = 0;
  uint _glHeight = 0;
  uint _glFormat = GL_UNSIGNED_INT_8_8_8_8_REV;

  auto (*glXSwapInterval)(int) -> int = nullptr;
};
