#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class InputDI : public Input {
public:
LPDIRECTINPUT8       di;
LPDIRECTINPUTDEVICE8 di_key, di_joy[INPUT_JOYMAX];
uint32               di_joy_count;

  void   poll_hw();
  void   poll();
  void   init();
  void   term();

  bool   enum_joypads(const DIDEVICEINSTANCE *instance);

  InputDI() {
    di     = 0;
    di_key = 0;
    for(int i = 0; i < INPUT_JOYMAX; i++)di_joy[i] = 0;
  }

  ~InputDI() { term(); }
};
