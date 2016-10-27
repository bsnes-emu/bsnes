#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  return {
    gdk_screen_get_width(gdk_screen_get_default()),
    gdk_screen_get_height(gdk_screen_get_default())
  };
}

auto pDesktop::workspace() -> Geometry {
  #if defined(DISPLAY_WINDOWS)
  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  return {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
  #endif

  #if defined(DISPLAY_XORG)
  auto display = XOpenDisplay(nullptr);
  int screen = DefaultScreen(display);

  int format;
  unsigned char* data = nullptr;
  unsigned long items, after;
  XlibAtom returnAtom;

  int result = XGetWindowProperty(
    display, RootWindow(display, screen), XInternAtom(display, "_NET_WORKAREA", XlibTrue), 0, 4, XlibFalse,
    XInternAtom(display, "CARDINAL", XlibTrue), &returnAtom, &format, &items, &after, &data
  );

  XCloseDisplay(display);

  if(result == Success && returnAtom == XInternAtom(display, "CARDINAL", XlibTrue) && format == 32 && items == 4) {
    unsigned long* workarea = (unsigned long*)data;
    return {(int)workarea[0], (int)workarea[1], (int)workarea[2], (int)workarea[3]};
  }

  return {
    0, 0,
    gdk_screen_get_width(gdk_screen_get_default()),
    gdk_screen_get_height(gdk_screen_get_default())
  };
  #endif
}

}

#endif
