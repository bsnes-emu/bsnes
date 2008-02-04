#include <GL/gl.h>
#include <GL/glx.h>

#include <ruby/ruby.h>

namespace ruby {

#include "glx.h"

class pVideoGLX {
public:
  VideoGLX &self;
  uint16_t *buffer;

  Display *display;
  int screen;
  XWindowAttributes attributes;
  GLXFBConfig fbconfig, *fbconfiglist;
  GLXContext glxcontext;
  GLXWindow glxwindow;
  GLuint gltexture;

  struct {
    bool double_buffer;
    unsigned buffer_size;
    unsigned red_size;
    unsigned green_size;
    unsigned blue_size;
    unsigned alpha_size;
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

  bool lock(uint16_t *&data, unsigned &pitch) {
    pitch = 1024 * 2;
    return data = buffer;
  }

  void unlock() {
  }

  void clear() {
    memset(buffer, 0, 1024 * 1024 * sizeof(uint16_t));
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    if(glx.double_buffer) glXSwapBuffers(display, glxwindow);
  }

  void refresh(unsigned width_, unsigned height_) {
    XGetWindowAttributes(display, settings.handle, &attributes);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      settings.filter == Video::FilterPoint ? GL_NEAREST : GL_LINEAR);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, attributes.width, 0, attributes.height, -1.0, 1.0);
    glViewport(0, 0, attributes.width, attributes.height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 1024); //length of buffer in pixels
    glTexSubImage2D(GL_TEXTURE_2D,
      /* mip-map level = */ 0, /* x = */ 0, /* y = */ 0,
      width_, height_, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);

    //OpenGL projection sets 0,0 as *bottom-left* of screen
    //Therefore, below vertices flip image to support top-left source
    //Texture range = x1:0.0, y1:0.0, x2:1.0, y2:1.0
    //Vertex range = x1:0, y1:0, x2:width, y2:height
    double w = double(width_)  / 1024.0;
    double h = double(height_) / 1024.0;
    int u = attributes.width;
    int v = attributes.height;
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
    buffer = new(zeromemory) uint16_t[1024 * 1024 * 4];

    //GLX requires a compatible X11 visualid to render to.
    //glXChooseFBConfig often chooses visualid that is not
    //compatible with target X11 window's visualid.
    //Therefore, iterate all available framebuffer configurations
    //to locate matching visualid.

    display = XOpenDisplay(0);
    screen = DefaultScreen(display);
    XGetWindowAttributes(display, settings.handle, &attributes);

    int elements = 0;
    fbconfig = 0;
    fbconfiglist = glXGetFBConfigs(display, screen, &elements);
    for(int i = 0; i < elements; i++) {
      int visualid = 0;
      glXGetFBConfigAttrib(display, fbconfiglist[i], GLX_VISUAL_ID, &visualid);
      if(visualid == attributes.visual->visualid) {
        fbconfig = fbconfiglist[i];
        break;
      }
    }

    if(!fbconfig) {
      fprintf(stderr, "VideoGLX: failed to find compatible Visual ID\n");
      term();
      return false;
    }

    glxcontext = glXCreateNewContext(display, fbconfig, GLX_RGBA_TYPE, 0, false);
    if(!glxcontext) {
      fprintf(stderr, "VideoGLX: failed to create context\n");
      term();
      return false;
    }

    glxwindow = glXCreateWindow(display, fbconfig, settings.handle, 0);
    if(!glxwindow) {
      fprintf(stderr, "VideoGLX: failed to create window\n");
      term();
      return false;
    }

    if(glXMakeContextCurrent(display, glxwindow, glxwindow, glxcontext) == false) {
      fprintf(stderr, "VideoGLX: failed to set context\n");
      term();
      return false;
    }

    //read attributes of frame buffer for later use
    int value = 0;
    glXGetFBConfigAttrib(display, fbconfig, GLX_DOUBLEBUFFER, &value);
    glx.double_buffer = value;
    glXGetFBConfigAttrib(display, fbconfig, GLX_BUFFER_SIZE, &value);
    glx.buffer_size = value;
    glXGetFBConfigAttrib(display, fbconfig, GLX_RED_SIZE, &value);
    glx.red_size = value;
    glXGetFBConfigAttrib(display, fbconfig, GLX_GREEN_SIZE, &value);
    glx.green_size = value;
    glXGetFBConfigAttrib(display, fbconfig, GLX_BLUE_SIZE, &value);
    glx.blue_size = value;
    glXGetFBConfigAttrib(display, fbconfig, GLX_ALPHA_SIZE, &value);
    glx.alpha_size = value;

    //disable unused features, enable required features
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_SMOOTH);
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
      /* format = */ GL_RGB, GL_UNSIGNED_SHORT_5_6_5, buffer);

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

    delete[] buffer;
  }

  pVideoGLX(VideoGLX &self_) : self(self_) {
    settings.handle = 0;
    glxcontext = 0;
    glxwindow = 0;
    gltexture = 0;
  }
};

bool VideoGLX::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoGLX::get(Setting setting) { return p.get(setting); }
bool VideoGLX::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoGLX::lock(uint16_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoGLX::unlock() { p.unlock(); }
void VideoGLX::clear() { p.clear(); }
void VideoGLX::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoGLX::init() { return p.init(); }
void VideoGLX::term() { p.term(); }
VideoGLX::VideoGLX() : p(*new pVideoGLX(*this)) {}
VideoGLX::~VideoGLX() { delete &p; }

} //namespace ruby
