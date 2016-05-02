//Xorg/GLX OpenGL 2.0 driver

//note: this is a fallback driver for use when OpenGL 3.2 is not available.
//see glx.cpp for comments on how this driver operates (they are very similar.)

struct VideoGLX2 : Video {
  ~VideoGLX2() { term(); }

  auto (*glXSwapInterval)(signed) -> signed = nullptr;
  Display* display = nullptr;
  signed screen = 0;
  Window xwindow = 0;
  Colormap colormap = 0;
  GLXContext glxcontext = nullptr;
  GLXWindow glxwindow = 0;

  struct {
    Window handle = 0;
    bool synchronize = false;
    unsigned filter = 1;  //linear

    unsigned width = 256;
    unsigned height = 256;

    bool isDoubleBuffered = false;
    bool isDirect = false;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
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

    if(name == Video::Filter && value.is<unsigned>()) {
      settings.filter = value.get<unsigned>();
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    if(width != settings.width || height != settings.height) resize(width, height);
    pitch = glwidth * sizeof(uint32_t);
    return data = glbuffer;
  }

  auto unlock() -> void {
  }

  auto clear() -> void {
    memory::fill(glbuffer, glwidth * glheight * sizeof(uint32_t));
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    if(settings.isDoubleBuffered) glXSwapBuffers(display, glxwindow);
  }

  auto refresh() -> void {
    XWindowAttributes parent, child;
    XGetWindowAttributes(display, settings.handle, &parent);
    XGetWindowAttributes(display, xwindow, &child);
    if(child.width != parent.width || child.height != parent.height) {
      XResizeWindow(display, xwindow, parent.width, parent.height);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filter ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filter ? GL_LINEAR : GL_NEAREST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, parent.width, 0, parent.height, -1.0, 1.0);
    glViewport(0, 0, parent.width, parent.height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPixelStorei(GL_UNPACK_ROW_LENGTH, glwidth);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, settings.width, settings.height,
      GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, glbuffer);

    double w = (double)settings.width / (double)glwidth;
    double h = (double)settings.height / (double)glheight;
    signed u = parent.width;
    signed v = parent.height;

    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0, 0); glVertex3i(0, v, 0);
    glTexCoord2f(w, 0); glVertex3i(u, v, 0);
    glTexCoord2f(0, h); glVertex3i(0, 0, 0);
    glTexCoord2f(w, h); glVertex3i(u, 0, 0);
    glEnd();
    glFlush();

    if(settings.isDoubleBuffered) glXSwapBuffers(display, glxwindow);
  }

  auto init() -> bool {
    display = XOpenDisplay(0);
    screen = DefaultScreen(display);

    signed versionMajor = 0, versionMinor = 0;
    glXQueryVersion(display, &versionMajor, &versionMinor);
    if(versionMajor < 1 || (versionMajor == 1 && versionMinor < 2)) return false;

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display, settings.handle, &windowAttributes);

    signed attributeList[] = {
      GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
      GLX_RENDER_TYPE, GLX_RGBA_BIT,
      GLX_DOUBLEBUFFER, True,
      GLX_RED_SIZE, 8,
      GLX_GREEN_SIZE, 8,
      GLX_BLUE_SIZE, 8,
      None
    };

    signed fbCount = 0;
    auto fbConfig = glXChooseFBConfig(display, screen, attributeList, &fbCount);
    if(fbCount == 0) return false;

    auto vi = glXGetVisualFromFBConfig(display, fbConfig[0]);
    colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    XSetWindowAttributes attributes;
    attributes.colormap = colormap;
    attributes.border_pixel = 0;
    xwindow = XCreateWindow(display, settings.handle, 0, 0, windowAttributes.width, windowAttributes.height,
      0, vi->depth, InputOutput, vi->visual, CWColormap | CWBorderPixel, &attributes);
    XSetWindowBackground(display, xwindow, 0);
    XMapWindow(display, xwindow);
    XFlush(display);

    while(XPending(display)) {
      XEvent event;
      XNextEvent(display, &event);
    }

    glxcontext = glXCreateContext(display, vi, 0, GL_TRUE);
    glXMakeCurrent(display, glxwindow = xwindow, glxcontext);

    if(!glXSwapInterval) glXSwapInterval = (signed (*)(signed))glGetProcAddress("glXSwapIntervalMESA");
    if(!glXSwapInterval) glXSwapInterval = (signed (*)(signed))glGetProcAddress("glXSwapIntervalSGI");

    if(glXSwapInterval) glXSwapInterval(settings.synchronize);

    signed value = 0;
    glXGetConfig(display, vi, GLX_DOUBLEBUFFER, &value);
    settings.isDoubleBuffered = value;
    settings.isDirect = glXIsDirect(display, glxcontext);

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_DITHER);
    glEnable(GL_TEXTURE_2D);

    resize(256, 256);
    return true;
  }

  auto term() -> void {
    if(gltexture) {
      glDeleteTextures(1, &gltexture);
      gltexture = 0;
    }

    if(glbuffer) {
      delete[] glbuffer;
      glbuffer = 0;
    }

    glwidth = 0;
    glheight = 0;

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

private:
  GLuint gltexture = 0;
  uint32_t* glbuffer = nullptr;
  unsigned glwidth = 0;
  unsigned glheight = 0;

  auto resize(unsigned width, unsigned height) -> void {
    settings.width = width;
    settings.height = height;

    if(gltexture == 0) glGenTextures(1, &gltexture);
    glwidth = max(glwidth, width);
    glheight = max(glheight, height);
    if(glbuffer) delete[] glbuffer;
    glbuffer = new uint32_t[glwidth * glheight]();

    glBindTexture(GL_TEXTURE_2D, gltexture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, glwidth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glwidth, glheight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, glbuffer);
  }
};
