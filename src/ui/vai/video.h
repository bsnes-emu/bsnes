#ifndef VIDEO_H
#define VIDEO_H

#include "../../lib/bbase.h"

class Video {
public:
  enum Setting {
    Handle,
    Synchronize,
    Filter,
  };

  enum Filter {
    FilterPoint,
    FilterLinear,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual bool lock(uint16 *&data, uint &pitch) { return false; }
  virtual void unlock() {}

  virtual void clear_video() {}
  virtual void refresh(uint width, uint height) {}
  virtual void init() {}
  virtual void term() {}

  Video() {}
  virtual ~Video() {}
};

#endif //ifndef VIDEO_H
