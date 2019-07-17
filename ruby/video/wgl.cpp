#include "opengl/opengl.hpp"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

static LRESULT CALLBACK VideoOpenGL32_WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

struct VideoWGL : VideoDriver, OpenGL {
  VideoWGL& self = *this;
  VideoWGL(Video& super) : VideoDriver(super) { construct(); }
  ~VideoWGL() { destruct(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "OpenGL 3.2"; }
  auto ready() -> bool override { return _ready; }

  auto hasExclusive() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFlush() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setExclusive(bool exclusive) -> bool override {
    return initialize();
  }

  auto setContext(uintptr context) -> bool override {
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(wglSwapInterval) wglSwapInterval(blocking);
    return true;
  }

  auto setFlush(bool flush) -> bool override {
    return true;
  }

  auto setShader(string shader) -> bool override {
    OpenGL::setShader(self.shader);
    return true;
  }

  auto clear() -> void override {
    OpenGL::clear();
    SwapBuffers(_display);
  }

  auto size(uint& width, uint& height) -> void {
    RECT rectangle;
    GetClientRect(_context, &rectangle);
    width = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    OpenGL::size(width, height);
    return OpenGL::lock(data, pitch);
  }

  auto release() -> void override {
  }

  auto output(uint width, uint height) -> void override {
    uint windowWidth, windowHeight;
    size(windowWidth, windowHeight);
    OpenGL::absoluteWidth = width;
    OpenGL::absoluteHeight = height;
    OpenGL::outputWidth = windowWidth;
    OpenGL::outputHeight = windowHeight;
    OpenGL::output();
    SwapBuffers(_display);
    if(self.flush) glFinish();
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
    windowClass.lpfnWndProc = VideoOpenGL32_WindowProcedure;
    windowClass.lpszClassName = L"VideoOpenGL32_Window";
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
    _monitorWidth = information.rcMonitor.right - information.rcMonitor.left;
    _monitorHeight = information.rcMonitor.bottom - information.rcMonitor.top;

    if(self.exclusive) {
      _context = _exclusive = CreateWindowEx(WS_EX_TOPMOST, L"VideoOpenGL32_Window", L"", WS_VISIBLE | WS_POPUP,
        information.rcMonitor.left, information.rcMonitor.top, _monitorWidth, _monitorHeight,
        nullptr, nullptr, GetModuleHandle(0), nullptr);
    } else {
      _context = (HWND)self.context;
    }

    PIXELFORMATDESCRIPTOR descriptor{};
    descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    descriptor.nVersion = 1;
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    descriptor.iPixelType = PFD_TYPE_RGBA;

    _display = GetDC(_context);
    GLuint pixelFormat = ChoosePixelFormat(_display, &descriptor);
    SetPixelFormat(_display, pixelFormat, &descriptor);

    _wglContext = wglCreateContext(_display);
    wglMakeCurrent(_display, _wglContext);

    wglCreateContextAttribs = (HGLRC (APIENTRY*)(HDC, HGLRC, const int*))glGetProcAddress("wglCreateContextAttribsARB");
    wglSwapInterval = (BOOL (APIENTRY*)(int))glGetProcAddress("wglSwapIntervalEXT");

    if(wglCreateContextAttribs) {
      int attributeList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0
      };
      HGLRC context = wglCreateContextAttribs(_display, 0, attributeList);
      if(context) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(_wglContext);
        wglMakeCurrent(_display, _wglContext = context);
      }
    }

    if(wglSwapInterval) wglSwapInterval(self.blocking);
    return _ready = OpenGL::initialize(self.shader);
  }

  auto terminate() -> void {
    _ready = false;
    OpenGL::terminate();

    if(_wglContext) {
      wglDeleteContext(_wglContext);
      _wglContext = nullptr;
    }

    if(_exclusive) {
      DestroyWindow(_exclusive);
      _exclusive = nullptr;
    }

    _context = nullptr;
  }

  auto (APIENTRY* wglCreateContextAttribs)(HDC, HGLRC, const int*) -> HGLRC = nullptr;
  auto (APIENTRY* wglSwapInterval)(int) -> BOOL = nullptr;

  bool _ready = false;

  uint _monitorWidth = 0;
  uint _monitorHeight = 0;

  HWND _exclusive = nullptr;
  HWND _context = nullptr;
  HDC _display = nullptr;
  HGLRC _wglContext = nullptr;
  HINSTANCE _glWindow = nullptr;
};
