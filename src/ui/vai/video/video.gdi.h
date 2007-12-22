#ifndef VIDEO_GDI_H
#define VIDEO_GDI_H

#include "../video.h"

class pVideoGDI;

class VideoGDI : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoGDI();
  ~VideoGDI();

private:
  pVideoGDI &p;
};

#endif //ifndef VIDEO_GDI_H
