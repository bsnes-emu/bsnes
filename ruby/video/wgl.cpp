#include "opengl/opengl.hpp"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

struct VideoWGL : Video, OpenGL {
  VideoWGL() { initialize(); }
  ~VideoWGL() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return _context; }
  auto blocking() -> bool { return _blocking; }
  auto smooth() -> bool { return _smooth; }
  auto shader() -> string { return _shader; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;
    if(wglSwapInterval) wglSwapInterval(_blocking);
    return true;
  }

  auto setSmooth(bool smooth) -> bool {
    if(_smooth == smooth) return true;
    _smooth = smooth;
    if(!_shader) OpenGL::filter = _smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto setShader(string shader) -> bool {
    if(_shader == shader) return true;
    OpenGL::shader(_shader = shader);
    if(!_shader) OpenGL::filter = _smooth ? GL_LINEAR : GL_NEAREST;
    return true;
  }

  auto clear() -> void {
    if(!ready()) return;
    OpenGL::clear();
    SwapBuffers(_display);
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(!ready()) return false;
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto unlock() -> void {
    if(!ready()) return;
  }

  auto output() -> void {
    if(!ready()) return;
    RECT rectangle;
    GetClientRect((HWND)_context, &rectangle);
    OpenGL::outputWidth = rectangle.right - rectangle.left;
    OpenGL::outputHeight = rectangle.bottom - rectangle.top;
    OpenGL::output();
    SwapBuffers(_display);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    PIXELFORMATDESCRIPTOR descriptor = {};
    descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor.nVersion = 1;
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType = PFD_TYPE_RGBA;

    _display = GetDC((HWND)_context);
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

    if(wglSwapInterval) wglSwapInterval(_blocking);
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
  uintptr _context = 0;
  bool _blocking = false;
  bool _smooth = true;
  string _shader;

  HDC _display = nullptr;
  HGLRC _wglContext = nullptr;
  HWND _window = nullptr;
  HINSTANCE _glWindow = nullptr;
};
