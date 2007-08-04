#ifndef XINPUT_H
#define XINPUT_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>

class InputX : public Input { public:
  bool key_down(uint16 key);

  void clear_input();
  void poll();
  void init();
  void term();

private:
Display *display;
char keymap[32];
};

#endif
