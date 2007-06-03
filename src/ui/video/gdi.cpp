#include "gdi.h"

bool VideoGDI::lock(uint16 *&data, uint &pitch) {
  pitch = 2048;
  data  = buffer;
  return data;
}

void VideoGDI::unlock() {}

void VideoGDI::refresh(uint r_width, uint r_height) {
RECT rc;
  GetClientRect(hwnd, &rc);

  SetDIBits(bitmapdc, bitmap, 0, r_height, (void*)buffer, &bmi, DIB_RGB_COLORS);
HDC hdc = GetDC(hwnd);
  StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, bitmapdc, 0, 1024 - r_height, r_width, r_height, SRCCOPY);
  ReleaseDC(hwnd, hdc);
}

void VideoGDI::init() {
HDC hdc = GetDC(hwnd);
  bitmapdc = CreateCompatibleDC(hdc);
  assert(bitmapdc);
  bitmap = CreateCompatibleBitmap(hdc, 1024, 1024);
  assert(bitmap);
  SelectObject(bitmapdc, bitmap);
  ReleaseDC(hwnd, hdc);

  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       = 1024;
  bmi.bmiHeader.biHeight      = -1024;
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biBitCount    = 16;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage   = 1024 * 1024 * sizeof(uint16);

//note:
//biBitCount of 15 is invalid, biBitCount of 16 is really RGB555

  snes.set_video_pixel_format(SNES::PIXELFORMAT_RGB555);
}

void VideoGDI::term() {
  DeleteObject(bitmap);
  DeleteDC(bitmapdc);
}

VideoGDI::VideoGDI(HWND handle) {
  hwnd = handle;
  buffer = (uint16*)malloc(1024 * 1024 * sizeof(uint16));
}

VideoGDI::~VideoGDI() {
  safe_free(buffer);
}
