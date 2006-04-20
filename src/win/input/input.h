#define INPUT_JOYMAX 8

class Input {
public:

enum {
  EVENT_INPUTKEYDOWN = EVENT_USER + 0,
  EVENT_INPUTKEYUP   = EVENT_USER + 1,
};

//this must be initialized by init()
struct Keymap {
  uint esc;
  uint f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;
  uint print_screen, scroll_lock, pause;

  uint tilde;
  uint num_0, num_1, num_2, num_3, num_4;
  uint num_5, num_6, num_7, num_8, num_9;
  uint minus, plus, backspace;
  uint ins, del, home, end, page_up, page_down;

  uint a, b, c, d, e, f, g, h, i, j, k, l, m;
  uint n, o, p, q, r, s, t, u, v, w, x, y, z;

  uint lbracket, rbracket, pipe;
  uint colon, quote;
  uint comma, period, question;

  uint numpad_0, numpad_1, numpad_2, numpad_3, numpad_4;
  uint numpad_5, numpad_6, numpad_7, numpad_8, numpad_9;
  uint numpad_plus, numpad_minus, numpad_mul, numpad_div;
  uint numpad_enter, numpad_point;

  uint numlock, capslock;

  uint up, down, left, right;

  uint tab, enter, space;
  uint lctrl, rctrl;
  uint lalt, ralt;
  uint lshift, rshift;

  uint lwin, rwin, menu;
} keymap;

uint8     keystate[256],     joystate[INPUT_JOYMAX * 256];
uint8 old_keystate[256], old_joystate[INPUT_JOYMAX * 256];

enum {
//bit 31: clear = keypress, set = joypress
  KEYFLAG     = 0x00000000,
  JOYFLAG     = 0x80000000,

  JOYMASK     = 0xffff00,
  KEYMASK     = 0x0000ff,
  JOYKEYMASK  = 0xffffff,
  JOY_NONE    = 0xffff00,
  KEY_NONE    = 0x0000ff,
  JOYKEY_NONE = 0xffffff,
};

  virtual void poll() = 0;

  virtual void ui_poll_input(Window *focus, bool require_window_focus = true);
  virtual void ui_clear_input();

  virtual void poll(uint8 type) = 0;
  virtual bool get_status(uint8 device, uint8 button) = 0;
  virtual void set_status(uint8 device, uint8 button, bool status) = 0;
  virtual void clear_input() = 0;
  virtual void init();
  virtual void term();

//helper functions
  bool keydown(uint id);
  uint stringtokeymap(const char *str);
  const char *keymaptostring(uint key);

  Input();
};

#include "dinput.h"
