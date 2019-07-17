#include "opengl/opengl.hpp"

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

auto VideoGLX_X11ErrorHandler(Display*, XErrorEvent*) -> int {
  return 0;  //suppress errors
}

struct VideoGLX : VideoDriver, OpenGL {
  VideoGLX& self = *this;
  VideoGLX(Video& super) : VideoDriver(super) { construct(); }
  ~VideoGLX() { destruct(); }

  auto create() -> bool override {
    super.setFormat("RGB24");
    return initialize();
  }

  auto driver() -> string override { return "OpenGL 3.2"; }
  auto ready() -> bool override { return _ready; }

  auto hasExclusive() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto hasFormats() -> vector<string> override {
    if(_depth == 30) return {"RGB30", "RGB24"};
    if(_depth == 24) return {"RGB24"};
    return {"RGB24"};  //fallback
  }

  auto setExclusive(bool exclusive) -> bool override {
    return initialize();
  }

  auto setContext(uintptr context) -> bool override {
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(glXSwapInterval) glXSwapInterval(blocking);
    return true;
  }

  auto setFlush(bool flush) -> bool override {
    return true;
  }

  auto setFormat(string format) -> bool override {
    if(format == "RGB24") {
      OpenGL::inputFormat = GL_RGBA8;
      return initialize();
    }

    if(format == "RGB30") {
      OpenGL::inputFormat = GL_RGB10_A2;
      return initialize();
    }

    return false;
  }

  auto setShader(string shader) -> bool override {
    OpenGL::setShader(shader);
    return true;
  }

  auto clear() -> void override {
    OpenGL::clear();
    if(_doubleBuffer) glXSwapBuffers(_display, _glXWindow);
  }

  auto size(uint& width, uint& height) -> void override {
    XWindowAttributes window;
    XGetWindowAttributes(_display, _window, &window);

    XWindowAttributes parent;
    XGetWindowAttributes(_display, _parent, &parent);

    if(window.width != parent.width || window.height != parent.height) {
      XResizeWindow(_display, _window, parent.width, parent.height);
    }

    width = parent.width;
    height = parent.height;
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
  }

  auto output(uint width, uint height) -> void override {
    uint windowWidth, windowHeight;
    size(windowWidth, windowHeight);

    OpenGL::absoluteWidth = width;
    OpenGL::absoluteHeight = height;
    OpenGL::outputWidth = windowWidth;
    OpenGL::outputHeight = windowHeight;
    OpenGL::output();
    if(_doubleBuffer) glXSwapBuffers(_display, _glXWindow);
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
  auto construct() -> void {
    _display = XOpenDisplay(nullptr);
    _screen = DefaultScreen(_display);

    XWindowAttributes attributes{};
    XGetWindowAttributes(_display, RootWindow(_display, _screen), &attributes);
    _depth = attributes.depth;
  }

  auto destruct() -> void {
    terminate();

    if(_display) {
      XCloseDisplay(_display);
      _display = nullptr;
      _screen = 0;
    }
  }

  auto initialize() -> bool {
    terminate();
    if(!self.exclusive && !self.context) return false;

    //require GLX 1.2+ API
    glXQueryVersion(_display, &_versionMajor, &_versionMinor);
    if(_versionMajor < 1 || (_versionMajor == 1 && _versionMinor < 2)) return false;

    int redDepth   = VideoDriver::format == "RGB30" ? 10 : 8;
    int greenDepth = VideoDriver::format == "RGB30" ? 10 : 8;
    int blueDepth  = VideoDriver::format == "RGB30" ? 10 : 8;

    //let GLX determine the best Visual to use for GL output; provide a few hints
    //note: some video drivers will override double buffering attribute
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
    GLXFBConfig* fbConfig = glXChooseFBConfig(_display, _screen, attributeList, &fbCount);
    if(fbCount == 0) return false;

    auto visual = glXGetVisualFromFBConfig(_display, fbConfig[0]);

    _parent = self.exclusive ? RootWindow(_display, visual->screen) : (Window)self.context;
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(_display, _parent, &windowAttributes);

    //(Window)self.context has already been realized, most likely with DefaultVisual.
    //GLX requires that the GL output window has the same Visual as the GLX context.
    //it is not possible to change the Visual of an already realized (created) window.
    //therefore a new child window, using the same GLX Visual, must be created and binded to it.
    _colormap = XCreateColormap(_display, RootWindow(_display, visual->screen), visual->visual, AllocNone);
    XSetWindowAttributes attributes{};
    attributes.border_pixel = 0;
    attributes.colormap = _colormap;
    attributes.override_redirect = self.exclusive;
    _window = XCreateWindow(_display, _parent,
      0, 0, windowAttributes.width, windowAttributes.height,
      0, visual->depth, InputOutput, visual->visual,
      CWBorderPixel | CWColormap | CWOverrideRedirect, &attributes);
    XSelectInput(_display, _window, ExposureMask);
    XSetWindowBackground(_display, _window, 0);
    XMapWindow(_display, _window);
    XFlush(_display);

    //window must be realized (appear onscreen) before we make the context current
    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
    }

    _glXContext = glXCreateContext(_display, visual, 0, GL_TRUE);
    glXMakeCurrent(_display, _glXWindow = _window, _glXContext);

    //glXSwapInterval is used to toggle Vsync
    //note that the ordering is very important! MESA declares SGI, but the SGI function does nothing
    if(!glXSwapInterval) glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalMESA");
    if(!glXSwapInterval) glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalSGI");

    if(auto glXCreateContextAttribs = (auto (*)(Display*, GLXFBConfig, GLXContext, int, const int*) -> GLXContext)glGetProcAddress("glXCreateContextAttribsARB")) {
      int attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 2,
        None
      };

      //glXCreateContextAttribs tends to throw BadRequest errors instead of simply failing gracefully
      auto originalHandler = XSetErrorHandler(VideoGLX_X11ErrorHandler);
      auto context = glXCreateContextAttribs(_display, fbConfig[0], nullptr, true, attributes);
      XSync(_display, False);
      XSetErrorHandler(originalHandler);

      if(context) {
        glXMakeCurrent(_display, 0, nullptr);
        glXDestroyContext(_display, _glXContext);
        glXMakeCurrent(_display, _glXWindow, _glXContext = context);
      } else {
        //OpenGL 3.2+ not supported (most likely OpenGL 2.x)
        return false;
      }
    } else {
      //missing required glXCreateContextAtribs function
      return false;
    }

    if(glXSwapInterval) glXSwapInterval(self.blocking);

    //read attributes of frame buffer for later use, as requested attributes from above are not always granted
    int value = 0;
    glXGetConfig(_display, visual, GLX_DOUBLEBUFFER, &value);
    _doubleBuffer = value;
    _isDirect = glXIsDirect(_display, _glXContext);

    return _ready = OpenGL::initialize(self.shader);
  }

  auto terminate() -> void {
    _ready = false;
    OpenGL::terminate();

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
  }

  bool _ready = false;

  auto (*glXSwapInterval)(int) -> int = nullptr;

  Display* _display = nullptr;
  int _screen = 0;
  uint _depth = 24;  //depth of the default root window
  Window _parent = 0;
  Window _window = 0;
  Colormap _colormap = 0;
  GLXContext _glXContext = nullptr;
  GLXWindow _glXWindow = 0;

  int _versionMajor = 0;
  int _versionMinor = 0;
  bool _doubleBuffer = false;
  bool _isDirect = false;
};
