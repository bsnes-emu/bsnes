#include "../../lib/libkeymap.h"
#define INPUT_JOYMAX 16

class Input {
private:
char tmp[256];

public:

struct joypad {
  bool up, down, left, right;
  bool a, b, x, y, l, r;
  bool select, start;
} joypad1, joypad2;

keymap key; //this must be initialized by init()
bool8 keystate[4096];

  virtual void poll_hw() {}
  virtual void poll();
  virtual void init();
  virtual void term();

//helper functions
  bool get_status(uint8 device, uint8 button);
  void set_status(uint8 device, uint8 button, bool status);
  void clear_input();

  bool keydown(uint16 key);
  uint get_key(uint device, uint button);
  void set_key(uint device, uint button, uint key);

  uint decode(string&);
  const char *encode(uint);

  Input();
} *uiInput;
