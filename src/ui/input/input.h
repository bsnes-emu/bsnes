#ifndef INPUT_H
#define INPUT_H

class Input { public:
  virtual bool key_down(uint16 key) { return false; }
  virtual bool key_up  (uint16 key) { return !key_down(key); }

  virtual void signal_key_down(uint16 key) {}
  virtual void signal_key_up  (uint16 key) {}

  virtual void clear_input() {}
  virtual void poll() {}
  virtual void init() {}
  virtual void term() {}
} *uiInput;

#endif
