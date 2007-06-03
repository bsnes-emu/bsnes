#ifndef VIDEO_GDI_H
#define VIDEO_GDI_H

class VideoGDI : public Video {
private:
HWND hwnd;
uint16 *buffer;
HBITMAP bitmap;
HDC bitmapdc;
BITMAPINFO bmi;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoGDI(HWND handle);
  ~VideoGDI();
};

#endif
