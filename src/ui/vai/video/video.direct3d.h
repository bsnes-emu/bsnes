#ifndef VIDEO_DIRECT3D_H
#define VIDEO_DIRECT3D_H

#include "../video.h"

class pVideoD3D;

class VideoD3D : public Video {
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

  VideoD3D();
  ~VideoD3D();

private:
  pVideoD3D &p;
};

#endif //ifndef VIDEO_DIRECT3D_H
