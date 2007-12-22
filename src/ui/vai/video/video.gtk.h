#ifndef VIDEO_GTK_H
#define VIDEO_GTK_H

#include "../video.h"

class pVideoGTK;

class VideoGTK : public Video {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoGTK();
  ~VideoGTK();

private:
  pVideoGTK &p;
};

#endif //ifndef VIDEO_GTK_H
