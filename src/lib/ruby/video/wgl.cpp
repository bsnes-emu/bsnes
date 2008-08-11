/*
  video.wgl
  authors: byuu, krom, mudlord
  license: public domain
  date: 2008-03-26
*/

#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <ruby/ruby.h>

namespace ruby {

#include "wgl.h"

class pVideoWGL {
public:
  VideoWGL &self;
  uint32_t *buffer;

  HDC display;
  HGLRC wglcontext;
  HWND window;
  HINSTANCE glwindow;
  GLuint gltexture;

  struct {
    HWND handle;
    unsigned filter;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    if(setting == Video::Filter) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return (uintptr_t)settings.handle;
    if(setting == Video::Filter) return settings.filter;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = (HWND)param;
      return true;
    }
    if(setting == Video::Filter) {
      settings.filter = param;
      return true;
    }
    return false;
  }

  bool lock(uint32_t *&data, unsigned &pitch) {
    pitch = 1024 * 4;
    return data = buffer;
  }

  void unlock() {
  }

  void clear() {
    memset(buffer, 0, 1024 * 1024 * sizeof(uint32_t));
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    SwapBuffers(display);
  }

  void refresh(unsigned width, unsigned height) {
    RECT rc;
    GetClientRect(settings.handle, &rc);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glViewport(0, 0, rc.right, rc.bottom);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 1024); //length of buffer in pixels
    glTexSubImage2D(GL_TEXTURE_2D,
      /* mip-map level = */ 0, /* x = */ 0, /* y = */ 0,
      width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

    //OpenGL projection sets 0,0 as *bottom-left* of screen.
    //therefore, below vertices flip image to support top-left source.
    //texture range = x1:0.0, y1:0.0, x2:1.0, y2:1.0
    //vertex range = x1:0, y1:0, x2:width, y2:height
    double w = double(width)  / 1024.0;
    double h = double(height) / 1024.0;
    int u = width;
    int v = height;
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0, 0); glVertex3i(0, v, 0);
    glTexCoord2f(w, 0); glVertex3i(u, v, 0);
    glTexCoord2f(0, h); glVertex3i(0, 0, 0);
    glTexCoord2f(w, h); glVertex3i(u, 0, 0);
    glEnd();

    glFlush();
    SwapBuffers(display);
  }

  bool init() {
    buffer = new(zeromemory) uint32_t[1024 * 1024 * sizeof(uint32_t)];

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

    //disable unused features
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_STENCIL_TEST);

    //enable useful and required features
    glEnable(GL_DITHER);
    glEnable(GL_TEXTURE_2D);

    //create GL texture to copy buffer to
    gltexture = 0;
    glGenTextures(1, &gltexture);
    glBindTexture(GL_TEXTURE_2D, gltexture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 1024);
    glTexImage2D(GL_TEXTURE_2D,
      /* mip-map level = */ 0, /* internal format = */ GL_RGB,
      /* width = */ 1024, /* height = */ 1024, /* border = */ 0,
      /* format = */ GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

    return true;
  }

  void term() {
    if(gltexture) {
      glDeleteTextures(1, &gltexture);
      gltexture = 0;
    }

    if(wglcontext) {
      wglDeleteContext(wglcontext);
      wglcontext = 0;
    }

    if(buffer) {
      delete[] buffer;
      buffer = 0;
    }
  }

  pVideoWGL(VideoWGL &self_) : self(self_) {
    settings.handle = 0;
    window = 0;
    wglcontext = 0;
    glwindow = 0;
    gltexture = 0;
  }

  ~pVideoWGL() { term(); }
};

bool VideoWGL::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoWGL::get(Setting setting) { return p.get(setting); }
bool VideoWGL::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoWGL::lock(uint32_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoWGL::unlock() { p.unlock(); }
void VideoWGL::clear() { p.clear(); }
void VideoWGL::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoWGL::init() { return p.init(); }
void VideoWGL::term() { p.term(); }
VideoWGL::VideoWGL() : p(*new pVideoWGL(*this)) {}
VideoWGL::~VideoWGL() { delete &p; }

} //namespace ruby
