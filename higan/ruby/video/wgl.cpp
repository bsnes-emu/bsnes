#include "opengl/opengl.hpp"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

namespace ruby {

struct pVideoWGL : OpenGL {
  HGLRC (APIENTRY* wglCreateContextAttribs)(HDC, HGLRC, const int*) = nullptr;
  BOOL (APIENTRY* wglSwapInterval)(int) = nullptr;

  HDC display;
  HGLRC wglcontext;
  HWND window;
  HINSTANCE glwindow;

  struct {
    HWND handle;
    bool synchronize;
    unsigned filter;
    string shader;
  } settings;

  bool cap(const string& name) {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Video::Handle) {
      settings.handle = (HWND)any_cast<uintptr_t>(value);
      return true;
    }

    if(name == Video::Synchronize) {
      if(settings.synchronize != any_cast<bool>(value)) {
        settings.synchronize = any_cast<bool>(value);
        if(wglcontext) {
          init();
          OpenGL::shader(settings.shader);
          if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
        }
      }
    }

    if(name == Video::Filter) {
      settings.filter = any_cast<unsigned>(value);
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    if(name == Video::Shader) {
      settings.shader = any_cast<const char*>(value);
      OpenGL::shader(settings.shader);
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    return false;
  }

  bool lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  void unlock() {
  }

  void clear() {
    OpenGL::clear();
    SwapBuffers(display);
  }

  void refresh() {
    RECT rc;
    GetClientRect(settings.handle, &rc);
    outputWidth = rc.right - rc.left, outputHeight = rc.bottom - rc.top;
    OpenGL::refresh();
    SwapBuffers(display);
  }

  bool init() {
    term();

    GLuint pixel_format;
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
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
        wglMakeCurrent(NULL, NULL);
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

  void term() {
    OpenGL::term();

    if(wglcontext) {
      wglDeleteContext(wglcontext);
      wglcontext = 0;
    }
  }

  pVideoWGL() {
    settings.handle = 0;
    settings.synchronize = false;
    settings.filter = 0;

    window = 0;
    wglcontext = 0;
    glwindow = 0;
  }

  ~pVideoWGL() { term(); }
};

DeclareVideo(WGL)

};
