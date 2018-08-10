#if defined(Hiro_Window)

namespace hiro {

static auto CALLBACK Window_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(Application::state().quit) return DefWindowProc(hwnd, msg, wparam, lparam);

  if(auto window = (mWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto self = window->self()) {
      if(self->_modalityDisabled()) {
        return DefWindowProc(hwnd, msg, wparam, lparam);
      }
      if(auto result = self->windowProc(hwnd, msg, wparam, lparam)) {
        return result();
      }
    }
  }

  return Shared_windowProc(DefWindowProc, hwnd, msg, wparam, lparam);
}

static const uint FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN;
static const uint ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN;

uint pWindow::minimumStatusHeight = 0;

auto pWindow::initialize() -> void {
  pApplication::state().modalTimer.setInterval(1);
  pApplication::state().modalTimer.onActivate([] { Application::doMain(); });

  HWND hwnd = CreateWindow(L"hiroWindow", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  HWND hstatus = CreateWindow(STATUSCLASSNAME, L"", WS_CHILD, 0, 0, 0, 0, hwnd, nullptr, GetModuleHandle(0), 0);
  SetWindowPos(hstatus, nullptr, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
  RECT rc;
  GetWindowRect(hstatus, &rc);
  minimumStatusHeight = rc.bottom - rc.top;
  DestroyWindow(hstatus);
  DestroyWindow(hwnd);
}

auto pWindow::construct() -> void {
  hwnd = CreateWindow(L"hiroWindow", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  setDroppable(state().droppable);
  setGeometry({128, 128, 256, 256});

  windows.append(self().instance);
}

auto pWindow::destruct() -> void {
  if(auto position = windows.find(self().instance)) windows.remove(*position);

  if(hbrush) { DeleteObject(hbrush); hbrush = nullptr; }
  DestroyWindow(hwnd);
}

auto pWindow::append(sMenuBar menuBar) -> void {
}

auto pWindow::append(sSizable sizable) -> void {
}

auto pWindow::append(sStatusBar statusBar) -> void {
}

auto pWindow::focused() const -> bool {
  return (GetForegroundWindow() == hwnd);
}

auto pWindow::frameMargin() const -> Geometry {
  RECT rc{0, 0, 640, 480};
  uint style = state().fullScreen ? 0 : state().resizable ? ResizableStyle : FixedStyle;
  bool menuVisible = state().menuBar && state().menuBar->visible();
  AdjustWindowRect(&rc, style, menuVisible);
  return {abs(rc.left), abs(rc.top), (rc.right - rc.left) - 640, (rc.bottom - rc.top) + _statusHeight() - 480};
}

auto pWindow::monitor() const -> uint {
  //TODO
  return 0;
}

auto pWindow::remove(sMenuBar menuBar) -> void {
}

auto pWindow::remove(sSizable sizable) -> void {
}

auto pWindow::remove(sStatusBar statusBar) -> void {
}

auto pWindow::setBackgroundColor(Color color) -> void {
  hbrushColor = CreateRGB(color);
  if(hbrush) { DeleteObject(hbrush); hbrush = nullptr; }
  if(color) hbrush = CreateSolidBrush(hbrushColor);
}

auto pWindow::setDismissable(bool dismissable) -> void {
}

auto pWindow::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

auto pWindow::setEnabled(bool enabled) -> void {
  if(auto& sizable = state().sizable) {
    if(auto self = sizable->self()) self->setEnabled(sizable->enabled(true));
  }
}

auto pWindow::setFocused() -> void {
  if(!self().visible()) self().setVisible(true);
  SetFocus(hwnd);
}

auto pWindow::setFont(const Font& font) -> void {
  if(auto& sizable = state().sizable) {
    if(auto self = sizable->self()) self->setFont(sizable->font(true));
  }
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  auto lock = acquire();
  auto style = GetWindowLongPtr(hwnd, GWL_STYLE) & WS_VISIBLE;
  if(fullScreen) {
    windowedGeometry = self().geometry();
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEX info;
    memset(&info, 0, sizeof(MONITORINFOEX));
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &info);
    RECT rc = info.rcMonitor;
    Geometry geometry = {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
    SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_POPUP);
    Geometry margin = frameMargin();
    self().setGeometry({
      geometry.x() + margin.x(), geometry.y() + margin.y(),
      geometry.width() - margin.width(), geometry.height() - margin.height()
    });
  } else {
    SetWindowLongPtr(hwnd, GWL_STYLE, style | (state().resizable ? ResizableStyle : FixedStyle));
    self().setGeometry(windowedGeometry);
  }
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  auto lock = acquire();
  Geometry margin = frameMargin();
  SetWindowPos(
    hwnd, nullptr,
    geometry.x() - margin.x(), geometry.y() - margin.y(),
    geometry.width() + margin.width(), geometry.height() + margin.height(),
    SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED
  );
  if(auto& statusBar = state().statusBar) {
    if(auto self = statusBar->self()) {
      SetWindowPos(self->hwnd, nullptr, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
  }
  if(auto& sizable = state().sizable) {
    sizable->setGeometry(geometry.setPosition());
  }
}

auto pWindow::setMaximized(bool maximized) -> void {
  if(state().minimized) return;
  auto lock = acquire();
  ShowWindow(hwnd, maximized ? SW_MAXIMIZE : SW_SHOWNOACTIVATE);
}

auto pWindow::setMaximumSize(Size size) -> void {
}

auto pWindow::setMinimized(bool minimized) -> void {
  auto lock = acquire();
  ShowWindow(hwnd, minimized ? SW_MINIMIZE : state().maximized ? SW_MAXIMIZE : SW_SHOWNOACTIVATE);
}

auto pWindow::setMinimumSize(Size size) -> void {
}

//never call this directly: use Window::setModal() instead
//this function does not confirm the modality has actually changed before adjusting modalCount
auto pWindow::setModal(bool modality) -> void {
  if(modality) {
    modalIncrement();
    _modalityUpdate();
    while(state().modal) {
      Application::processEvents();
      if(!Application::state().onMain) usleep(20 * 1000);
    }
    _modalityUpdate();
  } else {
    modalDecrement();
  }
}

auto pWindow::setResizable(bool resizable) -> void {
  auto style = GetWindowLongPtr(hwnd, GWL_STYLE) & WS_VISIBLE;
  SetWindowLongPtr(hwnd, GWL_STYLE, style | (state().resizable ? ResizableStyle : FixedStyle));
  setGeometry(state().geometry);
}

auto pWindow::setTitle(string text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pWindow::setVisible(bool visible) -> void {
  auto lock = acquire();
  ShowWindow(hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
  if(auto& sizable = state().sizable) {
    sizable->setGeometry(self().geometry().setPosition());
  }
  if(!visible) self().setModal(false);
}

//

auto pWindow::modalIncrement() -> void {
  if(pApplication::state().modalCount++ == 0) {
    pApplication::state().modalTimer.setEnabled(true);
  }
}

auto pWindow::modalDecrement() -> void {
  if(--pApplication::state().modalCount == 0) {
    pApplication::state().modalTimer.setEnabled(false);
  }
}

auto pWindow::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_CLOSE || (msg == WM_KEYDOWN && wparam == VK_ESCAPE && state().dismissable)) {
    if(state().onClose) self().doClose();
    else self().setVisible(false);
    if(state().modal && !self().visible()) self().setModal(false);
    return true;
  }

  if(msg == WM_MOVE && !locked()) {
    state().geometry.setPosition(_geometry().position());
    self().doMove();
  }

  if(msg == WM_SIZE && !locked()) {
    if(auto statusBar = state().statusBar) {
      if(auto self = statusBar->self()) {
        SetWindowPos(self->hwnd, nullptr, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_FRAMECHANGED);
      }
    }
    state().geometry.setSize(_geometry().size());
    if(auto& sizable = state().sizable) {
      sizable->setGeometry(_geometry().setPosition());
    }
    self().doSize();
  }

  if(msg == WM_DROPFILES) {
    if(auto paths = DropPaths(wparam)) self().doDrop(paths);
    return false;
  }

  if(msg == WM_ERASEBKGND && hbrush) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    FillRect(ps.hdc, &rc, hbrush);
    EndPaint(hwnd, &ps);
    return true;
  }

  if(msg == WM_ENTERMENULOOP || msg == WM_ENTERSIZEMOVE) {
    modalIncrement();
    return false;
  }

  if(msg == WM_EXITMENULOOP || msg == WM_EXITSIZEMOVE) {
    modalDecrement();
    return false;
  }

  if(msg == WM_SYSCOMMAND) {
    if(wparam == SC_SCREENSAVE || wparam == SC_MONITORPOWER) {
      if(!Application::screenSaver()) return 0;
    }
  }

  return {};
}

//

auto pWindow::_geometry() -> Geometry {
  Geometry margin = frameMargin();

  RECT rc;
  if(IsIconic(hwnd)) {
    //GetWindowRect returns x=-32000,y=-32000 when window is minimized
    WINDOWPLACEMENT wp;
    GetWindowPlacement(hwnd, &wp);
    rc = wp.rcNormalPosition;
  } else {
    GetWindowRect(hwnd, &rc);
  }

  signed x = rc.left + margin.x();
  signed y = rc.top + margin.y();
  signed width = (rc.right - rc.left) - margin.width();
  signed height = (rc.bottom - rc.top) - margin.height();

  return {x, y, width, height};
}

auto pWindow::_modalityCount() -> unsigned {
  unsigned modalWindows = 0;
  for(auto& weak : windows) {
    if(auto object = weak.acquire()) {
      if(auto window = dynamic_cast<mWindow*>(object.data())) {
        if(window->modal()) modalWindows++;
      }
    }
  }
  return modalWindows;
}

auto pWindow::_modalityDisabled() -> bool {
  if(_modalityCount() == 0) return false;
  return !state().modal;
}

auto pWindow::_modalityUpdate() -> void {
  unsigned modalWindows = _modalityCount();
  for(auto& weak : windows) {
    if(auto object = weak.acquire()) {
      if(auto window = dynamic_cast<mWindow*>(object.data())) {
        if(auto self = window->self()) {
          bool enabled = !modalWindows || window->modal();
          if(IsWindowEnabled(self->hwnd) != enabled) {
            EnableWindow(self->hwnd, enabled);
          }
        }
      }
    }
  }
}

auto pWindow::_statusHeight() const -> int {
  int height = 0;
  if(auto& statusBar = state().statusBar) {
    if(statusBar->visible()) {
      auto& text = statusBar->state.text;
      height = statusBar->font(true).size(text ? text : " ").height();
      height = max(height, minimumStatusHeight);
    }
  }
  return height;
}

}

#endif
