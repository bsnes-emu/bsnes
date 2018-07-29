#if defined(Hiro_Window)

namespace hiro {

static const uint FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN;
static const uint ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN;

uint pWindow::minimumStatusHeight = 0;

auto pWindow::initialize() -> void {
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
  auto style = GetWindowLongPtr(hwnd, GWL_STYLE) & WS_VISIBLE;
  lock();
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
  unlock();
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  lock();
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
  unlock();
}

auto pWindow::setMaximized(bool maximized) -> void {
  if(state().minimized) return;
  lock();
  ShowWindow(hwnd, maximized ? SW_MAXIMIZE : SW_SHOWNOACTIVATE);
  unlock();
}

auto pWindow::setMaximumSize(Size size) -> void {
  //todo
}

auto pWindow::setMinimized(bool minimized) -> void {
  lock();
  ShowWindow(hwnd, minimized ? SW_MINIMIZE : state().maximized ? SW_MAXIMIZE : SW_SHOWNOACTIVATE);
  unlock();
}

auto pWindow::setMinimumSize(Size size) -> void {
  //todo
}

auto pWindow::setModal(bool modality) -> void {
  if(modality) {
    _modalityUpdate();
    while(state().modal) {
      Application::processEvents();
      if(Application::state.onMain) {
        Application::doMain();
      } else {
        usleep(20 * 1000);
      }
    }
    _modalityUpdate();
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
  if(!visible) setModal(false);
}

//

auto pWindow::onClose() -> void {
  if(state().onClose) self().doClose();
  else self().setVisible(false);
  if(state().modal && !self().visible()) self().setModal(false);
}

auto pWindow::onDrop(WPARAM wparam) -> void {
  auto paths = DropPaths(wparam);
  if(paths) self().doDrop(paths);
}

auto pWindow::onEraseBackground() -> bool {
  if(hbrush == 0) return false;
  RECT rc;
  GetClientRect(hwnd, &rc);
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  FillRect(ps.hdc, &rc, hbrush);
  EndPaint(hwnd, &ps);
  return true;
}

auto pWindow::onModalBegin() -> void {
  Application::Windows::doModalChange(true);
}

auto pWindow::onModalEnd() -> void {
  Application::Windows::doModalChange(false);
}

auto pWindow::onMove() -> void {
  if(locked()) return;
  state().geometry.setPosition(_geometry().position());
  self().doMove();
}

auto pWindow::onSize() -> void {
  if(locked()) return;
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

//

auto pWindow::_geometry() -> Geometry {
  Geometry margin = frameMargin();

  RECT rc;
  if(IsIconic(hwnd)) {
    //GetWindowRect returns -32000(x),-32000(y) when window is minimized
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
