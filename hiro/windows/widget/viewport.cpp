#if defined(Hiro_Viewport)

namespace hiro {

auto pViewport::construct() -> void {
  hwnd = CreateWindow(L"hiroWidget", L"",
    WS_CHILD | WS_DISABLED,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
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

auto pViewport::setFocusable(bool focusable) -> void {
  //handled by windowProc()
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

  if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP) {
    if(self().focusable()) return true;
  }

  if(msg == WM_ERASEBKGND) {
    return false;
  }

  if(msg == WM_PAINT) {
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

  if(msg == WM_LBUTTONDOWN) {
    if(self().focusable()) setFocused();
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

}

#endif
