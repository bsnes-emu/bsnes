struct VideoGDI : VideoDriver {
  VideoGDI& self = *this;
  VideoGDI(Video& super) : VideoDriver(super) {}
  ~VideoGDI() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "GDI"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override { return initialize(); }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!_buffer || _width != width || _height != height) {
      if(_buffer) delete[] _buffer;
      if(_bitmap) DeleteObject(_bitmap);
      if(_dc) DeleteObject(_dc);

      _buffer = new uint32_t[width * height]();
      _width = width;
      _height = height;

      HDC hdc = GetDC((HWND)self.context);
      _dc = CreateCompatibleDC(hdc);
      _bitmap = CreateCompatibleBitmap(hdc, width, height);
      SelectObject(_dc, _bitmap);
      ReleaseDC((HWND)self.context, hdc);

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
  }

  auto output() -> void override {
    RECT rc;
    GetClientRect((HWND)self.context, &rc);

    SetDIBits(_dc, _bitmap, 0, _height, (void*)_buffer, &_info, DIB_RGB_COLORS);
    HDC hdc = GetDC((HWND)self.context);
    StretchBlt(hdc, rc.left, rc.top, rc.right, rc.bottom, _dc, 0, 0, _width, _height, SRCCOPY);
    ReleaseDC((HWND)self.context, hdc);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!self.context) return false;

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
