static LRESULT CALLBACK VideoGDI_WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

struct VideoGDI : VideoDriver {
  VideoGDI& self = *this;
  VideoGDI(Video& super) : VideoDriver(super) { construct(); }
  ~VideoGDI() { destruct(); }

  auto create() -> bool override {
    super.setShader("None");
    return initialize();
  }

  auto driver() -> string override { return "GDI"; }
  auto ready() -> bool override { return _ready; }

  auto hasExclusive() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }

  auto setExclusive(bool exclusive) -> bool override { return initialize(); }
  auto setContext(uintptr context) -> bool override { return initialize(); }

  auto size(uint& width, uint& height) -> void override {
    RECT rectangle;
    GetClientRect(_context, &rectangle);
    width = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!_buffer || _width != width || _height != height) {
      if(_buffer) delete[] _buffer;
      if(_bitmap) DeleteObject(_bitmap);
      if(_dc) DeleteObject(_dc);

      _buffer = new uint32_t[width * height]();
      _width = width;
      _height = height;

      HDC hdc = GetDC(_context);
      _dc = CreateCompatibleDC(hdc);
      _bitmap = CreateCompatibleBitmap(hdc, width, height);
      SelectObject(_dc, _bitmap);
      ReleaseDC(_context, hdc);

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

  auto output(uint width, uint height) -> void override {
    uint windowWidth, windowHeight;
    size(windowWidth, windowHeight);

    SetDIBits(_dc, _bitmap, 0, _height, (void*)_buffer, &_info, DIB_RGB_COLORS);
    HDC hdc = GetDC(_context);
    StretchBlt(hdc,
      ((int)windowWidth - (int)width) / 2, ((int)windowHeight - (int)height) / 2, width, height, _dc,
      0, 0, _width, _height, SRCCOPY
    );
    ReleaseDC(_context, hdc);
  }

private:
  auto construct() -> void {
    WNDCLASS windowClass{};
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.lpfnWndProc = VideoGDI_WindowProcedure;
    windowClass.lpszClassName = L"VideoGDI_Window";
    windowClass.lpszMenuName = 0;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&windowClass);
  }

  auto destruct() -> void {
    terminate();
  }

  auto initialize() -> bool {
    terminate();
    if(!self.exclusive && !self.context) return false;

    POINT point{0, 0};
    HMONITOR monitor = MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEX information{};
    information.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &information);
    uint monitorWidth = information.rcMonitor.right - information.rcMonitor.left;
    uint monitorHeight = information.rcMonitor.bottom - information.rcMonitor.top;

    if(self.exclusive) {
      _context = _exclusive = CreateWindowEx(WS_EX_TOPMOST, L"VideoGDI_Window", L"", WS_VISIBLE | WS_POPUP,
        information.rcMonitor.left, information.rcMonitor.top, monitorWidth, monitorHeight,
        nullptr, nullptr, GetModuleHandle(0), nullptr);
    } else {
      _context = (HWND)self.context;
    }

    _width = 0;
    _height = 0;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_buffer) { delete[] _buffer; _buffer = nullptr; }
    if(_bitmap) { DeleteObject(_bitmap); _bitmap = nullptr; }
    if(_dc) { DeleteDC(_dc); _dc = nullptr; }
    if(_exclusive) { DestroyWindow(_exclusive); _exclusive = nullptr; }
    _context = nullptr;
  }

  bool _ready = false;

  uint32_t* _buffer = nullptr;
  uint _width = 0;
  uint _height = 0;

  HWND _exclusive = nullptr;
  HWND _context = nullptr;
  HBITMAP _bitmap = nullptr;
  HDC _dc = nullptr;
  BITMAPINFO _info = {};
};
