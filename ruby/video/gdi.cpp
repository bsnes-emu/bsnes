struct VideoGDI : Video {
  VideoGDI() { initialize(); }
  ~VideoGDI() { terminate(); }

  auto driver() -> string override { return "GDI"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    if(context == Video::context()) return true;
    if(!Video::setContext(context)) return false;
    return initialize();
  }

  auto clear() -> void override {
    if(!ready()) return;
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!ready()) return false;

    if(!_buffer || _width != width || _height != height) {
      if(_buffer) delete[] _buffer;
      if(_bitmap) DeleteObject(_bitmap);
      if(_dc) DeleteObject(_dc);

      _buffer = new uint32_t[width * height]();
      _width = width;
      _height = height;

      HDC hdc = GetDC((HWND)_context);
      _dc = CreateCompatibleDC(hdc);
      _bitmap = CreateCompatibleBitmap(hdc, width, height);
      SelectObject(_dc, _bitmap);
      ReleaseDC((HWND)_context, hdc);

      memory::fill(&_info, sizeof(BITMAPINFO));
      _info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      _info.bmiHeader.biWidth = width;
      _info.bmiHeader.biHeight = -height;
      _info.bmiHeader.biPlanes = 1;
      _info.bmiHeader.biBitCount = 32;
      _info.bmiHeader.biCompression = BI_RGB;
      _info.bmiHeader.biSizeImage = width * height * sizeof(uint32_t);
    }

    pitch = _width * sizeof(uint32_t);
    return data = _buffer;
  }

  auto release() -> void override {
    if(!ready()) return;
  }

  auto output() -> void override {
    if(!ready()) return;

    RECT rc;
    GetClientRect((HWND)_context, &rc);

    SetDIBits(_dc, _bitmap, 0, _height, (void*)_buffer, &_info, DIB_RGB_COLORS);
    HDC hdc = GetDC((HWND)_context);
    StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, _dc, 0, 0, _width, _height, SRCCOPY);
    ReleaseDC((HWND)_context, hdc);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    _width = 0;
    _height = 0;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_buffer) { delete[] _buffer; _buffer = nullptr; }
    if(_bitmap) { DeleteObject(_bitmap); _bitmap = nullptr; }
    if(_dc) { DeleteDC(_dc); _dc = nullptr; }
  }

  bool _ready = false;

  uint32_t* _buffer = nullptr;
  uint _width = 0;
  uint _height = 0;

  HBITMAP _bitmap = nullptr;
  HDC _dc = nullptr;
  BITMAPINFO _info = {};
};
