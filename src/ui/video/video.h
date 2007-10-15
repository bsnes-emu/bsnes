#ifndef VIDEO_H
#define VIDEO_H

class Video { public:

struct Settings {
  bool synchronize;
  uint filter;
  Settings() : synchronize(false), filter(0) {}
} settings;

enum Filter {
  FilterPoint,
  FilterLinear,
};

  virtual bool lock(uint16 *&data, uint &pitch) { return false; }
  virtual void unlock() {}

  virtual void clear_video() {}
  virtual void refresh(uint width, uint height) {}
  virtual void init() {}
  virtual void term() {}

  virtual void update_settings() {}

  Video() {}
  virtual ~Video() {}
} *uiVideo;

#endif
