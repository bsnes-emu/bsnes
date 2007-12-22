#ifndef VIDEO_XV_H
#define VIDEO_XV_H

#include "../video.h"

class pVideoXv;

class VideoXv : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  void clear_video();
  void refresh(uint width, uint height);
  void init();
  void term();

  VideoXv();
  ~VideoXv();

private:
  pVideoXv &p;
};

#endif //ifndef VIDEO_XV_H
