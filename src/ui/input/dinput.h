#ifndef DINPUT_H
#define DINPUT_H

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTINPUT_JOYMAX 16
#include <dinput.h>

class InputDI : public Input {
private:
uint8 keystate[65536];

public:
HWND owner;
LPDIRECTINPUT8 di;
LPDIRECTINPUTDEVICE8 di_key, di_joy[DIRECTINPUT_JOYMAX];
uint32 di_joy_count;

  void poll();
  void init();
  void term();
  bool key_down(uint16 key);

//
  uint16 translate(uint16 key);
  bool enum_joypads(const DIDEVICEINSTANCE *instance);

  InputDI(HWND hwnd) {
    owner  = hwnd;
    di     = 0;
    di_key = 0;
    for(int i = 0; i < DIRECTINPUT_JOYMAX; i++)di_joy[i] = 0;
  }

  ~InputDI() { term(); }
};

#endif
