#if defined(Hiro_Window)

namespace hiro {

vector<pWindow*> pWindow::modal;

static const unsigned FixedStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER;
static const unsigned ResizableStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;

auto pWindow::construct() -> void {
  hwnd = CreateWindow(L"hiroWindow", L"", ResizableStyle, 128, 128, 256, 256, 0, 0, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  setDroppable(state().droppable);
  setGeometry({128, 128, 256, 256});
}

auto pWindow::destruct() -> void {
  if(hbrush) { DeleteObject(hbrush); hbrush = nullptr; }
  DestroyWindow(hwnd);
}

auto pWindow::append(sLayout layout) -> void {
}

auto pWindow::append(sMenuBar menuBar) -> void {
}

auto pWindow::append(sStatusBar statusBar) -> void {
}

auto pWindow::focused() const -> bool override {
  return (GetForegroundWindow() == hwnd);
}

auto pWindow::frameMargin() const -> Geometry {
  unsigned style = state().resizable ? ResizableStyle : FixedStyle;
  if(state().fullScreen) style = 0;
  RECT rc{0, 0, 640, 480};
  AdjustWindowRect(&rc, style, (bool)GetMenu(hwnd));
  signed statusHeight = 0;
  if(auto statusBar = state().statusBar) {
    if(auto self = statusBar->self()) {
      if(statusBar->visible()) {
        RECT src;
        GetClientRect(self->hwnd, &src);
        statusHeight = src.bottom - src.top;
      }
    }
  }
  return {abs(rc.left), abs(rc.top), (rc.right - rc.left) - 640, (rc.bottom - rc.top) + statusHeight - 480};
}

auto pWindow::remove(sLayout layout) -> void {
}

auto pWindow::remove(sMenuBar menuBar) -> void {
}

auto pWindow::remove(sStatusBar statusBar) -> void {
}

auto pWindow::setBackgroundColor(Color color) -> void {
  hbrushColor = CreateRGB(color);
  if(hbrush) { DeleteObject(hbrush); hbrush = nullptr; }
  if(color) hbrush = CreateSolidBrush(hbrushColor);
}

auto pWindow::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

auto pWindow::setEnabled(bool enabled) -> void {
}

auto pWindow::setFocused() -> void {
  if(!self().visible()) self().setVisible(true);
  SetFocus(hwnd);
}

auto pWindow::setFullScreen(bool fullScreen) -> void {
  lock();
  if(fullScreen == false) {
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | (state().resizable ? ResizableStyle : FixedStyle));
    setGeometry(state().geometry);
  } else {
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEX info;
    memset(&info, 0, sizeof(MONITORINFOEX));
    info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &info);
    RECT rc = info.rcMonitor;
    Geometry geometry = {rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top};
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    Geometry margin = frameMargin();
    setGeometry({
      geometry.x() + margin.x(), geometry.y() + margin.y(),
      geometry.width() - margin.width(), geometry.height() - margin.height()
    });
  }
  unlock();
}

auto pWindow::setGeometry(Geometry geometry) -> void {
  lock();
  Geometry margin = frameMargin();
  SetWindowPos(
    hwnd, NULL,
    geometry.x() - margin.x(), geometry.y() - margin.y(),
    geometry.width() + margin.width(), geometry.height() + margin.height(),
    SWP_NOZORDER | SWP_FRAMECHANGED
  );
  if(auto statusBar = state().statusBar) {
    if(auto self = statusBar->self()) {
      SetWindowPos(self->hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
    }
  }
  if(auto layout = state().layout) {
    layout->setGeometry(geometry.setPosition(0, 0));
  }
  unlock();
}

auto pWindow::setModal(bool modality) -> void {
  if(modality) {
    modal.appendOnce(this);
    while(state().modal) {
      Application::processEvents();
      if(Application::state.onMain) {
        Application::doMain();
      } else {
        usleep(20 * 1000);
      }
    }
    if(auto position = modal.find(this)) modal.remove(position());
  }
}

auto pWindow::setResizable(bool resizable) -> void {
  SetWindowLongPtr(hwnd, GWL_STYLE, state().resizable ? ResizableStyle : FixedStyle);
  setGeometry(state().geometry);
}

auto pWindow::setTitle(string text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pWindow::setVisible(bool visible) -> void {
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
  lstring paths = DropPaths(wparam);
  if(paths.empty()) return;
  self().doDrop(paths);
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
      SetWindowPos(self->hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);
    }
  }
  state().geometry.setSize(_geometry().size());
  if(auto layout = state().layout) {
    layout->setGeometry(_geometry().setPosition(0, 0));
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

}

#endif
