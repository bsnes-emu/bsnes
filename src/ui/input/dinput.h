#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class InputDI : public Input {
public:
LPDIRECTINPUT8       di;
LPDIRECTINPUTDEVICE8 di_key, di_joy[INPUT_JOYMAX];
uint32               di_joy_count;

struct joypad {
  bool up, down, left, right;
  bool a, b, x, y, l, r;
  bool select, start;
} joypad1, joypad2;

  void   poll();
  bool   button_down(uint32 r);

  void   poll(uint8 type);
  bool   get_status(uint8 device, uint8 button);
  void   set_status(uint8 device, uint8 button, bool status);
  void   clear_input();
  bool   enum_joypads(const DIDEVICEINSTANCE *instance);
  void   init();
  void   term();

  InputDI() {
    di     = 0;
    di_key = 0;
    for(int i = 0; i < INPUT_JOYMAX; i++)di_joy[i] = 0;
  }

  ~InputDI() { term(); }
};
