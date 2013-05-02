namespace phoenix {

Size pDesktop::size() {
  return {
    gdk_screen_get_width(gdk_screen_get_default()),
    gdk_screen_get_height(gdk_screen_get_default())
  };
}

Geometry pDesktop::workspace() {
  XlibDisplay* display = XOpenDisplay(nullptr);
  int screen = DefaultScreen(display);

  static Atom atom = XlibNone;
  if(atom == XlibNone) atom = XInternAtom(display, "_NET_WORKAREA", True);

  int format;
  unsigned char* data = nullptr;
  unsigned long items, after;
  Atom returnAtom;

  int result = XGetWindowProperty(
    display, RootWindow(display, screen), atom, 0, 4, False, XA_CARDINAL, &returnAtom, &format, &items, &after, &data
  );

  XCloseDisplay(display);

  if(result == Success && returnAtom == XA_CARDINAL && format == 32 && items == 4) {
    unsigned long *workarea = (unsigned long*)data;
    return {(signed)workarea[0], (signed)workarea[1], (unsigned)workarea[2], (unsigned)workarea[3]};
  }

  return {
    0, 0,
    gdk_screen_get_width(gdk_screen_get_default()),
    gdk_screen_get_height(gdk_screen_get_default())
  };
}

}
