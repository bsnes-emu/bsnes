#include <assert.h>

namespace ruby {

struct pVideoGDI {
  uint32_t* buffer = nullptr;
  HBITMAP bitmap = nullptr;
  HDC bitmapdc = nullptr;
  BITMAPINFO bmi;

  struct {
    HWND handle = nullptr;

    unsigned width;
    unsigned height;
  } settings;

  pVideoGDI() {
    buffer = (uint32_t*)memory::allocate(1024 * 1024 * sizeof(uint32_t));
  }

  ~pVideoGDI() {
    if(buffer) memory::free(buffer);
    term();
  }

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = (HWND)value.get<uintptr_t>();
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    settings.width  = width;
    settings.height = height;

    pitch = 1024 * 4;
    return data = buffer;
  }

  auto unlock() -> void {}

  auto clear() -> void {}

  auto refresh() -> void {
    RECT rc;
    GetClientRect(settings.handle, &rc);

    SetDIBits(bitmapdc, bitmap, 0, settings.height, (void*)buffer, &bmi, DIB_RGB_COLORS);
    HDC hdc = GetDC(settings.handle);
    StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, bitmapdc, 0, 1024 - settings.height, settings.width, settings.height, SRCCOPY);
    ReleaseDC(settings.handle, hdc);
  }

  auto init() -> bool {
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
    bmi.bmiHeader.biBitCount    = 32; //biBitCount of 15 is invalid, biBitCount of 16 is really RGB555
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage   = 1024 * 1024 * sizeof(uint32_t);

    settings.width  = 256;
    settings.height = 256;
    return true;
  }

  auto term() -> void {
    DeleteObject(bitmap);
    DeleteDC(bitmapdc);
  }
};

DeclareVideo(GDI)

};
