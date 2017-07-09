#include "opengl/opengl.hpp"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

struct VideoWGL : Video, OpenGL {
  ~VideoWGL() { term(); }

  auto (APIENTRY* wglCreateContextAttribs)(HDC, HGLRC, const int*) -> HGLRC = nullptr;
  auto (APIENTRY* wglSwapInterval)(int) -> BOOL = nullptr;

  HDC display = nullptr;
  HGLRC wglcontext = nullptr;
  HWND window = nullptr;
  HINSTANCE glwindow = nullptr;

  struct {
    HWND handle = nullptr;
    bool synchronize = false;
    uint filter = Video::FilterNearest;
    string shader;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = (HWND)value.get<uintptr_t>();
      return true;
    }

    if(name == Video::Synchronize && value.is<bool>()) {
      if(settings.synchronize != value.get<bool>()) {
        settings.synchronize = value.get<bool>();
        if(wglcontext) {
          init();
          OpenGL::shader(settings.shader);
          if(!settings.shader) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
        }
      }
    }

    if(name == Video::Filter && value.is<uint>()) {
      settings.filter = value.get<uint>();
      if(!settings.shader) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    if(name == Video::Shader && value.is<string>()) {
      settings.shader = value.get<string>();
      OpenGL::shader(settings.shader);
      if(!settings.shader) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto unlock() -> void {
  }

  auto clear() -> void {
    OpenGL::clear();
    SwapBuffers(display);
  }

  auto refresh() -> void {
    RECT rc;
    GetClientRect(settings.handle, &rc);
    outputWidth = rc.right - rc.left, outputHeight = rc.bottom - rc.top;
    OpenGL::refresh();
    SwapBuffers(display);
  }

  auto init() -> bool {
    GLuint pixel_format;
    PIXELFORMATDESCRIPTOR pfd;
    memory::fill(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;

    display = GetDC(settings.handle);
    pixel_format = ChoosePixelFormat(display, &pfd);
    SetPixelFormat(display, pixel_format, &pfd);

    wglcontext = wglCreateContext(display);
    wglMakeCurrent(display, wglcontext);

    wglCreateContextAttribs = (HGLRC (APIENTRY*)(HDC, HGLRC, const int*))glGetProcAddress("wglCreateContextAttribsARB");
    wglSwapInterval = (BOOL (APIENTRY*)(int))glGetProcAddress("wglSwapIntervalEXT");

    if(wglCreateContextAttribs) {
      int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0
      };
      HGLRC context = wglCreateContextAttribs(display, 0, attributes);
      if(context) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(wglcontext);
        wglMakeCurrent(display, wglcontext = context);
      }
    }

    if(wglSwapInterval) {
      wglSwapInterval(settings.synchronize);
    }

    OpenGL::init();
    return true;
  }

  auto term() -> void {
    OpenGL::term();

    if(wglcontext) {
      wglDeleteContext(wglcontext);
      wglcontext = nullptr;
    }
  }
};
