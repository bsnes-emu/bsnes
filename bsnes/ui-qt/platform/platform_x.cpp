#define None XNone
#define Window XWindow
#include <X11/Xlib.h>
#undef None
#undef Window

struct LibXtst : public library {
  function<int (Display*, unsigned int, Bool, unsigned long)> XTestFakeKeyEvent;

  LibXtst() {
    if(open("Xtst")) {
      XTestFakeKeyEvent = sym("XTestFakeKeyEvent");
    }
  }
} libXtst;

void supressScreenSaver() {
  if(!libXtst.XTestFakeKeyEvent) return;

  //XSetScreenSaver(timeout = 0) does not work
  //XResetScreenSaver() does not work
  //XScreenSaverSuspend() does not work
  //DPMSDisable() does not work
  //XSendEvent(KeyPressMask) does not work
  //use XTest extension to send fake keypress every ~20 seconds.
  //keycode of 255 does not map to any actual key,
  //but it will block screensaver and power management.
  Display *display = XOpenDisplay(0);
  libXtst.XTestFakeKeyEvent(display, 255, True,  0);
  libXtst.XTestFakeKeyEvent(display, 255, False, 0);
  XCloseDisplay(display);
}

