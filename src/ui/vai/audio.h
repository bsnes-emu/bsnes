#ifndef AUDIO_H
#define AUDIO_H

#include "../../lib/bbase.h"

class Audio {
public:
  enum Setting {
    Handle,
    Synchronize,
    Frequency,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual void sample(uint16 left, uint16 right) {}
  virtual void clear_audio() {}
  virtual void init() {}
  virtual void term() {}

  Audio() {}
  virtual ~Audio() {}
};

#endif //ifndef AUDIO_H
