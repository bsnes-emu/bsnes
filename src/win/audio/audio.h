class Audio {
public:
uint32 frequency;
  virtual void run(uint32 sample) = 0;
  virtual void set_frequency(uint32 new_freq) = 0;
  virtual void clear_audio() = 0;
  virtual void init() = 0;
  virtual void term() = 0;

  Audio() { frequency = 32000; }
};

#include "dsound.h"
