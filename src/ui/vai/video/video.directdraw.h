#ifndef VIDEO_DIRECTDRAW_H
#define VIDEO_DIRECTDRAW_H

#include "../video.h"

class pVideoDD;

class VideoDD : public Video {
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

  VideoDD();
  ~VideoDD();

private:
  pVideoDD &p;
};

#endif //ifndef VIDEO_DIRECTDRAW_H
