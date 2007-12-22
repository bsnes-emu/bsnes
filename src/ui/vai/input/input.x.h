#ifndef INPUT_X_H
#define INPUT_X_H

#include "../input.h"

class pInputX;

class InputX : public Input {
public:
  bool key_down(uint16 key);

  void clear_input();
  void poll();
  void init();
  void term();

  InputX();
  ~InputX();

private:
  pInputX &p;
};

#endif //ifndef INPUT_X_H
