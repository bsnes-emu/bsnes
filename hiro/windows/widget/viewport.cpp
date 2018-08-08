#if defined(Hiro_Viewport)

namespace hiro {

auto pViewport::construct() -> void {
  hwnd = CreateWindow(L"hiroWidget", L"",
    WS_CHILD | WS_DISABLED,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setDroppable(state().droppable);
}

auto pViewport::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pViewport::handle() const -> uintptr_t {
  return (uintptr_t)hwnd;
}

auto pViewport::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

//

auto pViewport::doMouseLeave() -> void {
  return self().doMouseLeave();
}

auto pViewport::doMouseMove(int x, int y) -> void {
  return self().doMouseMove({x, y});
}

auto pViewport::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_DROPFILES) {
    if(auto paths = DropPaths(wparam)) self().doDrop(paths);
    return false;
  }

  if(msg == WM_ERASEBKGND) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    auto brush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rc{};
    GetClientRect(hwnd, &rc);
    FillRect(ps.hdc, &rc, brush);
    DeleteObject(brush);
    EndPaint(hwnd, &ps);
    return true;
  }

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    switch(msg) {
    case WM_LBUTTONDOWN: self().doMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: self().doMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: self().doMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    switch(msg) {
    case WM_LBUTTONUP: self().doMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: self().doMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: self().doMouseRelease(Mouse::Button::Right); break;
    }
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

}

#endif
