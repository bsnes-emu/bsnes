/*
  video.glx
  author: byuu
  license: public domain
  date: 2008-02-06

  Design notes:
  SGI's GLX is the X11/Xlib interface to OpenGL.
  At the time of this writing, there are three relevant versions of the API: versions 1.2, 1.3 and 1.4.

  Version 1.2 was released on March 4th, 1997.
  Version 1.3 was released on October 19th, 1998.
  Version 1.4 was released on December 16th, 2005.

  Despite version 1.3 being roughly ten years old at this time, there are still many modern X11 GLX drivers
  that lack full support for the specification. Most notable would be the official video drivers from ATI.
  Given this, 1.4 support is pretty much hopeless to target.

  Luckily, each version has been designed to be backwards compatible with the previous version. As well,
  version 1.2 is wholly sufficient, albeit less convenient, to implement this video module.

  Therefore, for the purpose of compatibility, this driver only uses GLX 1.2 or earlier API commands.
  As well, it only uses raw Xlib API commands, so that it is compatible with any toolkit.
*/

#include <GL/gl.h>
#include <GL/glx.h>

#include <ruby/ruby.h>

namespace ruby {

#include "glx.h"

//returns true once window is mapped (created and displayed onscreen)
static Bool x_wait_for_map_notify(Display *d, XEvent *e, char *arg) {
  return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}

class pVideoGLX {
public:
  VideoGLX &self;
  uint32_t *buffer;

  Display *display;
  int screen;
  Window xwindow;
  GLXContext glxcontext;
  GLXWindow glxwindow;
  GLuint gltexture;

  struct {
    int version_major, version_minor;
    bool double_buffer;
    bool is_direct;
  } glx;

  struct {
    Window handle;
    unsigned filter;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    if(setting == Video::Filter) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return settings.handle;
    if(setting == Video::Filter) return settings.filter;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = param;
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
    if(glx.double_buffer) glXSwapBuffers(display, glxwindow);
  }

  void refresh(unsigned width, unsigned height) {
    //we must ensure that the child window is the same size as the parent window.
    //unfortunately, we cannot hook the parent window resize event notification,
    //as we did not create the parent window, nor have any knowledge of the toolkit used.
    //therefore, inelegant as it may be, we query each window size and resize as needed.
    XWindowAttributes parent, child;
    XGetWindowAttributes(display, settings.handle, &parent);
    XGetWindowAttributes(display, xwindow, &child);
    if(child.width != parent.width || child.height != parent.height) {
      XResizeWindow(display, xwindow, parent.width, parent.height);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, parent.width, 0, parent.height, -1.0, 1.0);
    glViewport(0, 0, parent.width, parent.height);

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
    int u = parent.width;
    int v = parent.height;
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0, 0); glVertex3i(0, v, 0);
    glTexCoord2f(w, 0); glVertex3i(u, v, 0);
    glTexCoord2f(0, h); glVertex3i(0, 0, 0);
    glTexCoord2f(w, h); glVertex3i(u, 0, 0);
    glEnd();

    glFlush();
    if(glx.double_buffer) glXSwapBuffers(display, glxwindow);
  }

  bool init() {
    display = XOpenDisplay(0);
    screen = DefaultScreen(display);
    glXQueryVersion(display, &glx.version_major, &glx.version_minor);
    //require GLX 1.2+ API
    if(glx.version_major < 1 || (glx.version_major == 1 && glx.version_minor < 2)) return false;

    buffer = new(zeromemory) uint32_t[1024 * 1024 * sizeof(uint32_t)];

    XWindowAttributes wa;
    XGetWindowAttributes(display, settings.handle, &wa);

    //let GLX determine the best Visual to use for GL output; provide a few hints
    int elements = 0;
    int attributelist[] = {
      GLX_RGBA,
      GLX_DOUBLEBUFFER,
      None
    };
    XVisualInfo *vi = glXChooseVisual(display, screen, attributelist);

    //Window settings.handle has already been realized, most likely with DefaultVisual.
    //GLX requires that the GL output window has the same Visual as the GLX context.
    //it is not possible to change the Visual of an already realized (created) window.
    //therefore a new child window, using the same GLX Visual, must be created and binded to settings.handle.
    Colormap colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = colormap;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;
    xwindow = XCreateWindow(display, /* parent = */ settings.handle,
      /* x = */ 0, /* y = */ 0, wa.width, wa.height,
      /* border_width = */ 0, vi->depth, InputOutput, vi->visual,
      CWColormap | CWBorderPixel | CWEventMask, &swa);
    XMapWindow(display, xwindow);
    XEvent event;
    XIfEvent(display, &event, x_wait_for_map_notify, (char*)xwindow); //wait for window to appear onscreen

    glxcontext = glXCreateContext(display, vi, /* sharelist = */ 0, /* direct = */ GL_TRUE);
    glXMakeCurrent(display, glxwindow = xwindow, glxcontext);

    //read attributes of frame buffer for later use, as requested attributes from above are not always granted
    int value = 0;
    glXGetConfig(display, vi, GLX_DOUBLEBUFFER, &value);
    glx.double_buffer = value;
    glx.is_direct = glXIsDirect(display, glxcontext);

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

    if(glxcontext) {
      glXDestroyContext(display, glxcontext);
      glxcontext = 0;
    }

    if(buffer) {
      delete[] buffer;
      buffer = 0;
    }
  }

  pVideoGLX(VideoGLX &self_) : self(self_) {
    settings.handle = 0;
    xwindow = 0;
    glxcontext = 0;
    glxwindow = 0;
    gltexture = 0;
  }

  ~pVideoGLX() { term(); }
};

bool VideoGLX::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoGLX::get(Setting setting) { return p.get(setting); }
bool VideoGLX::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoGLX::lock(uint32_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoGLX::unlock() { p.unlock(); }
void VideoGLX::clear() { p.clear(); }
void VideoGLX::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoGLX::init() { return p.init(); }
void VideoGLX::term() { p.term(); }
VideoGLX::VideoGLX() : p(*new pVideoGLX(*this)) {}
VideoGLX::~VideoGLX() { delete &p; }

} //namespace ruby
