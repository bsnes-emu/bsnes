#ifndef INPUT_DIRECTINPUT_H
#define INPUT_DIRECTINPUT_H

#include "../input.h"

class pInputDI;

class InputDI : public Input {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool key_down(uint16 key);

  void clear_input();
  void poll();
  void init();
  void term();

  InputDI();
  ~InputDI();

private:
  pInputDI &p;
};

#endif //ifndef INPUT_DIRECTINPUT_H
