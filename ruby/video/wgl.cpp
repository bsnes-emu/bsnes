#include "opengl/opengl.hpp"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

struct VideoWGL : VideoDriver, OpenGL {
  VideoWGL& self = *this;
  VideoWGL(Video& super) : VideoDriver(super) {}
  ~VideoWGL() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "OpenGL 3.2"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasSmooth() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(wglSwapInterval) wglSwapInterval(blocking);
    return true;
  }

  auto setFlush(bool flush) -> bool override {
    return true;
  }

  auto setSmooth(bool) -> bool override {
    if(!self.shader) OpenGL::filter = self.smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto setShader(string) -> bool override {
    OpenGL::setShader(self.shader);
    if(!self.shader) OpenGL::filter = self.smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto clear() -> void override {
    OpenGL::clear();
    SwapBuffers(_display);
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
  }

  auto output() -> void override {
    RECT rectangle;
    GetClientRect((HWND)self.context, &rectangle);
    OpenGL::outputWidth = rectangle.right - rectangle.left;
    OpenGL::outputHeight = rectangle.bottom - rectangle.top;
    OpenGL::output();
    SwapBuffers(_display);
    if(self.flush) glFinish();
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!self.context) return false;

    PIXELFORMATDESCRIPTOR descriptor = {};
    descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor.nVersion = 1;
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType = PFD_TYPE_RGBA;

    _display = GetDC((HWND)self.context);
    GLuint pixelFormat = ChoosePixelFormat(_display, &descriptor);
    SetPixelFormat(_display, pixelFormat, &descriptor);

    _wglContext = wglCreateContext(_display);
    wglMakeCurrent(_display, _wglContext);

    wglCreateContextAttribs = (HGLRC (APIENTRY*)(HDC, HGLRC, const int*))glGetProcAddress("wglCreateContextAttribsARB");
    wglSwapInterval = (BOOL (APIENTRY*)(int))glGetProcAddress("wglSwapIntervalEXT");

    if(wglCreateContextAttribs) {
      int attributeList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0
      };
      HGLRC context = wglCreateContextAttribs(_display, 0, attributeList);
      if(context) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(_wglContext);
        wglMakeCurrent(_display, _wglContext = context);
      }
    }

    if(wglSwapInterval) wglSwapInterval(self.blocking);
    return _ready = OpenGL::initialize();
  }

  auto terminate() -> void {
    _ready = false;
    OpenGL::terminate();

    if(_wglContext) {
      wglDeleteContext(_wglContext);
      _wglContext = nullptr;
    }
  }

  auto (APIENTRY* wglCreateContextAttribs)(HDC, HGLRC, const int*) -> HGLRC = nullptr;
  auto (APIENTRY* wglSwapInterval)(int) -> BOOL = nullptr;

  bool _ready = false;

  HDC _display = nullptr;
  HGLRC _wglContext = nullptr;
  HINSTANCE _glWindow = nullptr;
};
