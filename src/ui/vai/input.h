#ifndef INPUT_H
#define INPUT_H

#include "../../lib/bbase.h"
#include "../../lib/bkeymap.h"

class Input {
public:
  enum Setting {
    Handle,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual bool key_down(uint16 key) { return false; }
  virtual bool key_up  (uint16 key) { return !key_down(key); }

  virtual void clear_input() {}
  virtual void poll() {}
  virtual void init() {}
  virtual void term() {}

  Input() {}
  virtual ~Input() {}
};

#endif //ifndef INPUT_H
