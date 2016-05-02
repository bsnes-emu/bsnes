#include "opengl/opengl.hpp"

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

auto VideoGLX_X11ErrorHandler(Display*, XErrorEvent*) -> int {
  return 0;  //suppress errors
}

struct VideoGLX : Video, OpenGL {
  ~VideoGLX() { term(); }

  auto (*glXSwapInterval)(signed) -> signed = nullptr;

  Display* display = nullptr;
  signed screen = 0;
  Window xwindow = 0;
  Colormap colormap = 0;
  GLXContext glxcontext = nullptr;
  GLXWindow glxwindow = 0;

  struct {
    signed versionMajor = 0;
    signed versionMinor = 0;
    bool doubleBuffer = false;
    bool isDirect = false;
  } glx;

  struct {
    Window handle = 0;
    bool synchronize = false;
    unsigned depth = 24;
    unsigned filter = 1;  //linear
    string shader;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Depth) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Depth) return settings.depth;
    if(name == Video::Filter) return settings.filter;
    if(name == Video::Shader) return settings.shader;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = value.get<uintptr_t>();
      return true;
    }

    if(name == Video::Synchronize && value.is<bool>()) {
      if(settings.synchronize != value.get<bool>()) {
        settings.synchronize = value.get<bool>();
        if(glXSwapInterval) glXSwapInterval(settings.synchronize);
        return true;
      }
    }

    if(name == Video::Depth && value.is<unsigned>()) {
      unsigned depth = value.get<unsigned>();
      if(depth > DefaultDepth(display, screen)) return false;

      switch(depth) {
      case 24: inputFormat = GL_RGBA8; break;
      case 30: inputFormat = GL_RGB10_A2; break;
      default: return false;
      }

      settings.depth = depth;
      return true;
    }

    if(name == Video::Filter && value.is<unsigned>()) {
      settings.filter = value.get<unsigned>();
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    if(name == Video::Shader && value.is<string>()) {
      settings.shader = value.get<string>();
      OpenGL::shader(settings.shader);
      if(settings.shader.empty()) OpenGL::filter = settings.filter ? GL_LINEAR : GL_NEAREST;
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto unlock() -> void {
  }

  auto clear() -> void {
    OpenGL::clear();
    if(glx.doubleBuffer) glXSwapBuffers(display, glxwindow);
  }

  auto refresh() -> void {
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
    if(glx.doubleBuffer) glXSwapBuffers(display, glxwindow);
  }

  auto init() -> bool {
    display = XOpenDisplay(0);
    screen = DefaultScreen(display);

    //require GLX 1.2+ API
    glXQueryVersion(display, &glx.versionMajor, &glx.versionMinor);
    if(glx.versionMajor < 1 || (glx.versionMajor == 1 && glx.versionMinor < 2)) return false;

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display, settings.handle, &windowAttributes);

    //let GLX determine the best Visual to use for GL output; provide a few hints
    //note: some video drivers will override double buffering attribute
    signed attributeList[] = {
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DOUBLEBUFFER, True,
      GLX_RED_SIZE, (signed)(settings.depth / 3),
      GLX_GREEN_SIZE, (signed)(settings.depth / 3) + (signed)(settings.depth % 3),
      GLX_BLUE_SIZE, (signed)(settings.depth / 3),
      None
    };

    signed fbCount = 0;
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
      /* x = */ 0, /* y = */ 0, windowAttributes.width, windowAttributes.height,
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

    //glXSwapInterval is used to toggle Vsync
    //note that the ordering is very important! MESA declares SGI, but the SGI function does nothing
    if(!glXSwapInterval) glXSwapInterval = (signed (*)(signed))glGetProcAddress("glXSwapIntervalMESA");
    if(!glXSwapInterval) glXSwapInterval = (signed (*)(signed))glGetProcAddress("glXSwapIntervalSGI");

    if(auto glXCreateContextAttribs = (auto (*)(Display*, GLXFBConfig, GLXContext, signed, const signed*) -> GLXContext)glGetProcAddress("glXCreateContextAttribsARB")) {
      signed attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 2,
        None
      };

      //glXCreateContextAttribs tends to throw BadRequest errors instead of simply failing gracefully
      auto originalHandler = XSetErrorHandler(VideoGLX_X11ErrorHandler);
      auto context = glXCreateContextAttribs(display, fbConfig[0], nullptr, true, attributes);
      XSync(display, False);
      XSetErrorHandler(originalHandler);

      if(context) {
        glXMakeCurrent(display, 0, nullptr);
        glXDestroyContext(display, glxcontext);
        glXMakeCurrent(display, glxwindow, glxcontext = context);
      } else {
        //OpenGL 3.2+ not supported (most likely OpenGL 2.x)
        return false;
      }
    } else {
      //missing required glXCreateContextAtribs function
      return false;
    }

    if(glXSwapInterval) glXSwapInterval(settings.synchronize);

    //read attributes of frame buffer for later use, as requested attributes from above are not always granted
    signed value = 0;
    glXGetConfig(display, vi, GLX_DOUBLEBUFFER, &value);
    glx.doubleBuffer = value;
    glx.isDirect = glXIsDirect(display, glxcontext);

    OpenGL::init();
    return true;
  }

  auto term() -> void {
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

    if(display) {
      XCloseDisplay(display);
      display = nullptr;
    }
  }
};
