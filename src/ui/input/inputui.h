/*****
 * InputUI
 *
 * Input wrapper to capture UI key presses. Useful for when OS does not
 * support direct input polling methods (eg Xorg, etc)
 *****/

#ifndef INPUTUI_H
#define INPUTUI_H

class InputUI : public Input {
private:
bool keystate[65536];

public:
  void init() {
    Input::init();
    memset(keystate, 0, sizeof(keystate));
  }

  bool key_down(uint16 key) { return keystate[key]; }
  void signal_key_down(uint16 key) { keystate[key] = true;  }
  void signal_key_up  (uint16 key) { keystate[key] = false; }
};

#endif
