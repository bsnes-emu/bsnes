#include "opengl/opengl.hpp"

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

namespace ruby {

struct pVideoGLX : OpenGL {
  GLXContext (*glXCreateContextAttribs)(Display*, GLXFBConfig, GLXContext, int, const int*) = nullptr;
  int (*glXSwapInterval)(int) = nullptr;

  Display* display;
  int screen;
  Window xwindow;
  Colormap colormap;
  GLXContext glxcontext;
  GLXWindow glxwindow;

  struct {
    int version_major, version_minor;
    bool double_buffer;
    bool is_direct;
  } glx;

  struct {
    Window handle;
    bool synchronize;
    unsigned depth;
    unsigned filter;
    string shader;
  } settings;

  bool cap(const string& name) {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Depth) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Depth) return settings.depth;
    if(name == Video::Filter) return settings.filter;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Video::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }

    if(name == Video::Synchronize) {
      if(settings.synchronize != any_cast<bool>(value)) {
        settings.synchronize = any_cast<bool>(value);
        if(glXSwapInterval) glXSwapInterval(settings.synchronize);
        return true;
      }
    }

    if(name == Video::Depth) {
      unsigned depth = any_cast<unsigned>(value);
      if(depth > DefaultDepth(display, screen)) return false;

      switch(depth) {
      case 24: format = GL_RGBA8; break;
      case 30: format = GL_RGB10_A2; break;
      default: return false;
      }

      settings.depth = depth;
      return true;
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
    if(glx.double_buffer) glXSwapBuffers(display, glxwindow);
  }

  void refresh() {
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

    outputWidth = parent.width, outputHeight = parent.height;
    OpenGL::refresh();
    if(glx.double_buffer) glXSwapBuffers(display, glxwindow);
  }

  bool init() {
    term();

    glXQueryVersion(display, &glx.version_major, &glx.version_minor);
    //require GLX 1.2+ API
    if(glx.version_major < 1 || (glx.version_major == 1 && glx.version_minor < 2)) return false;

    XWindowAttributes window_attributes;
    XGetWindowAttributes(display, settings.handle, &window_attributes);

    //let GLX determine the best Visual to use for GL output; provide a few hints
    //note: some video drivers will override double buffering attribute
    int attributeList[] = {
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DOUBLEBUFFER, True,
      GLX_RED_SIZE, (signed)(settings.depth / 3),
      GLX_GREEN_SIZE, (signed)(settings.depth / 3) + (signed)(settings.depth % 3),
      GLX_BLUE_SIZE, (signed)(settings.depth / 3),
      None
    };

    int fbCount;
    GLXFBConfig* fbConfig = glXChooseFBConfig(display, screen, attributeList, &fbCount);
    if(fbCount == 0) return false;

    XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbConfig[0]);

    //Window settings.handle has already been realized, most likely with DefaultVisual.
    //GLX requires that the GL output window has the same Visual as the GLX context.
    //it is not possible to change the Visual of an already realized (created) window.
    //therefore a new child window, using the same GLX Visual, must be created and binded to settings.handle.
    colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes attributes;
    attributes.colormap = colormap;
    attributes.border_pixel = 0;
    xwindow = XCreateWindow(display, /* parent = */ settings.handle,
      /* x = */ 0, /* y = */ 0, window_attributes.width, window_attributes.height,
      /* border_width = */ 0, vi->depth, InputOutput, vi->visual,
      CWColormap | CWBorderPixel, &attributes);
    XSetWindowBackground(display, xwindow, /* color = */ 0);
    XMapWindow(display, xwindow);
    XFlush(display);

    //window must be realized (appear onscreen) before we make the context current
    while(XPending(display)) {
      XEvent event;
      XNextEvent(display, &event);
    }

    glxcontext = glXCreateContext(display, vi, /* sharelist = */ 0, /* direct = */ GL_TRUE);
    glXMakeCurrent(display, glxwindow = xwindow, glxcontext);

    glXCreateContextAttribs = (GLXContext (*)(Display*, GLXFBConfig, GLXContext, int, const int*))glGetProcAddress("glXCreateContextAttribsARB");
    glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalSGI");
    if(!glXSwapInterval) glXSwapInterval = (int (*)(int))glGetProcAddress("glXSwapIntervalMESA");

    if(glXCreateContextAttribs) {
      int attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 2,
        None
      };
      GLXContext context = glXCreateContextAttribs(display, fbConfig[0], nullptr, true, attributes);
      if(context) {
        glXMakeCurrent(display, 0, nullptr);
        glXDestroyContext(display, glxcontext);
        glXMakeCurrent(display, glxwindow, glxcontext = context);
      }
    }

    if(glXSwapInterval) {
      glXSwapInterval(settings.synchronize);
    }

    //read attributes of frame buffer for later use, as requested attributes from above are not always granted
    int value = 0;
    glXGetConfig(display, vi, GLX_DOUBLEBUFFER, &value);
    glx.double_buffer = value;
    glx.is_direct = glXIsDirect(display, glxcontext);

    OpenGL::init();
    return true;
  }

  void term() {
    OpenGL::term();

    if(glxcontext) {
      glXDestroyContext(display, glxcontext);
      glxcontext = nullptr;
    }

    if(xwindow) {
      XUnmapWindow(display, xwindow);
      xwindow = 0;
    }

    if(colormap) {
      XFreeColormap(display, colormap);
      colormap = 0;
    }
  }

  pVideoGLX() {
    display = XOpenDisplay(0);
    screen = DefaultScreen(display);

    settings.handle = 0;
    settings.synchronize = false;
    settings.depth = 24;
    settings.filter = 1;  //linear

    xwindow = 0;
    colormap = 0;
    glxcontext = nullptr;
    glxwindow = 0;
  }

  ~pVideoGLX() {
    term();
    XCloseDisplay(display);
  }
};

DeclareVideo(GLX)

};
