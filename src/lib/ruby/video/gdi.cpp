#include <assert.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <ruby/ruby.h>

namespace ruby {

#include "gdi.h"

class pVideoGDI {
public:
  VideoGDI &self;

  uint16_t *buffer;
  HBITMAP bitmap;
  HDC bitmapdc;
  BITMAPINFO bmi;

  struct {
    HWND handle;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return (uintptr_t)settings.handle;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = (HWND)param;
      return true;
    }
    return false;
  }

  bool lock(uint16_t *&data, unsigned &pitch) {
    pitch = 2048;
    return data = buffer;
  }

  void unlock() {}

  void refresh(unsigned r_width, unsigned r_height) {
  RECT rc;
    GetClientRect(settings.handle, &rc);

    SetDIBits(bitmapdc, bitmap, 0, r_height, (void*)buffer, &bmi, DIB_RGB_COLORS);
  HDC hdc = GetDC(settings.handle);
    StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, bitmapdc, 0, 1024 - r_height, r_width, r_height, SRCCOPY);
    ReleaseDC(settings.handle, hdc);
  }

  bool init() {
    HDC hdc = GetDC(settings.handle);
    bitmapdc = CreateCompatibleDC(hdc);
    assert(bitmapdc);
    bitmap = CreateCompatibleBitmap(hdc, 1024, 1024);
    assert(bitmap);
    SelectObject(bitmapdc, bitmap);
    ReleaseDC(settings.handle, hdc);

    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = 1024;
    bmi.bmiHeader.biHeight      = -1024;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 16; //biBitCount of 15 is invalid, biBitCount of 16 is really RGB555
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = 1024 * 1024 * sizeof(uint16_t);

    return true;
  }

  void term() {
    DeleteObject(bitmap);
    DeleteDC(bitmapdc);
  }

  pVideoGDI(VideoGDI &self_) : self(self_) {
    buffer = (uint16_t*)malloc(1024 * 1024 * sizeof(uint16_t));
    settings.handle = 0;
  }

  ~pVideoGDI() {
    if(buffer) free(buffer);
  }
};

bool VideoGDI::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoGDI::get(Setting setting) { return p.get(setting); }
bool VideoGDI::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoGDI::lock(uint16_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoGDI::unlock() { p.unlock(); }
void VideoGDI::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoGDI::init() { return p.init(); }
void VideoGDI::term() { p.term(); }
VideoGDI::VideoGDI() : p(*new pVideoGDI(*this)) {}
VideoGDI::~VideoGDI() { delete &p; }

} //namespace ruby
