#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class InputDI : public Input {
public:
LPDIRECTINPUT8       di;
LPDIRECTINPUTDEVICE8 di_key, di_joy;
uint8 keystate[256], joystate[256];

struct joypad {
  bool up, down, left, right;
  bool a, b, x, y, l, r;
  bool select, start;
} joypad1, joypad2;

  void   poll_devices();
  uint32 poll();

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
    di_joy = 0;
  }

  ~InputDI() { term(); }
};
