#include <assert.h>

struct VideoGDI : Video {
  ~VideoGDI() { term(); }

  struct Device {
    HBITMAP bitmap = nullptr;
    HDC dc = nullptr;
    BITMAPINFO info = {};
  } device;

  struct Settings {
    HWND handle = nullptr;

    uint32_t* buffer = nullptr;
    uint width = 0;
    uint height = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr>()) {
      settings.handle = (HWND)value.get<uintptr>();
      return true;
    }

    return false;
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(!settings.buffer || settings.width != width || settings.height != height) {
      if(settings.buffer) {
        delete[] settings.buffer;
        DeleteObject(device.bitmap);
        DeleteObject(device.dc);
      }
      settings.buffer = new uint32_t[width * height]();
      settings.width = width;
      settings.height = height;

      HDC hdc = GetDC(settings.handle);
      device.dc = CreateCompatibleDC(hdc);
      assert(device.dc);
      device.bitmap = CreateCompatibleBitmap(hdc, width, height);
      assert(device.bitmap);
      SelectObject(device.dc, device.bitmap);
      ReleaseDC(settings.handle, hdc);

      memory::fill(&device.info, sizeof(BITMAPINFO));
      device.info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      device.info.bmiHeader.biWidth = width;
      device.info.bmiHeader.biHeight = -height;
      device.info.bmiHeader.biPlanes = 1;
      device.info.bmiHeader.biBitCount = 32;
      device.info.bmiHeader.biCompression = BI_RGB;
      device.info.bmiHeader.biSizeImage = width * height * sizeof(uint32_t);
    }

    data = settings.buffer;
    pitch = settings.width * sizeof(uint32_t);
    return true;
  }

  auto unlock() -> void {}

  auto clear() -> void {}

  auto refresh() -> void {
    RECT rc;
    GetClientRect(settings.handle, &rc);

    SetDIBits(device.dc, device.bitmap, 0, settings.height, (void*)settings.buffer, &device.info, DIB_RGB_COLORS);
    HDC hdc = GetDC(settings.handle);
    StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, device.dc, 0, 0, settings.width, settings.height, SRCCOPY);
    ReleaseDC(settings.handle, hdc);
  }

  auto init() -> bool {
    settings.width  = 0;
    settings.height = 0;
    return true;
  }

  auto term() -> void {
    if(settings.buffer) {
      delete[] settings.buffer;
      DeleteObject(device.bitmap);
      DeleteDC(device.dc);
      settings.buffer = nullptr;
      device.bitmap = nullptr;
      device.dc = nullptr;
    }
  }
};
