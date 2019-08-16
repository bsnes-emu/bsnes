#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  #if defined(DISPLAY_WINDOWS)
  return {GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN)};
  #elif defined(DISPLAY_XORG)
  auto display = XOpenDisplay(nullptr);
  int screen = DefaultScreen(display);
  XWindowAttributes attributes;
  XGetWindowAttributes(display, RootWindow(display, screen), &attributes);
  XCloseDisplay(display);
  return {attributes.width, attributes.height};
  #else
  //this only returns the geometry of the primary monitor rather than the entire desktop
  QRect rect = QApplication::desktop()->screenGeometry();
  return {rect.width(), rect.height()};
  #endif
}

auto pDesktop::workspace() -> Geometry {
  #if defined(DISPLAY_WINDOWS)
  RECT rc;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
  return {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
  #elif defined(DISPLAY_XORG)
  auto display = XOpenDisplay(nullptr);
  int screen = DefaultScreen(display);

  int format;
  unsigned char* data = nullptr;
  unsigned long items, after;
  XlibAtom returnAtom;

  XlibAtom netWorkarea = XInternAtom(display, "_NET_WORKAREA", XlibTrue);
  int result = XGetWindowProperty(
    display, RootWindow(display, screen), netWorkarea, 0, 4, XlibFalse,
    XInternAtom(display, "CARDINAL", XlibTrue), &returnAtom, &format, &items, &after, &data
  );

  XlibAtom cardinal = XInternAtom(display, "CARDINAL", XlibTrue);
  if(result == Success && returnAtom == cardinal && format == 32 && items == 4) {
    unsigned long* workarea = (unsigned long*)data;
    XCloseDisplay(display);
    return {(int)workarea[0], (int)workarea[1], (int)workarea[2], (int)workarea[3]};
  }

  XCloseDisplay(display);
  auto size = Desktop::size();
  return {0, 0, size.width(), size.height()};
  #else
  //this only returns the workspace of the primary monitor rather than the entire desktop
  QRect rect = QApplication::desktop()->availableGeometry();
  return {rect.x(), rect.y(), rect.width(), rect.height()};
  #endif
}

}

#endif
