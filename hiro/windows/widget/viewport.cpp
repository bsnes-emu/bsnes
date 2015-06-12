#if defined(Hiro_Viewport)

namespace hiro {

static auto CALLBACK Viewport_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto viewport = dynamic_cast<mViewport*>(object);
  if(!viewport) return DefWindowProc(hwnd, msg, wparam, lparam);

  if(msg == WM_DROPFILES) {
    if(auto paths = DropPaths(wparam)) viewport->doDrop(paths);
    return false;
  }

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT tracker{sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd};
    TrackMouseEvent(&tracker);
    viewport->doMouseMove({(int16_t)LOWORD(lparam), (int16_t)HIWORD(lparam)});
  }

  if(msg == WM_MOUSELEAVE) {
    viewport->doMouseLeave();
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    switch(msg) {
    case WM_LBUTTONDOWN: viewport->doMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: viewport->doMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: viewport->doMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    switch(msg) {
    case WM_LBUTTONUP: viewport->doMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: viewport->doMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: viewport->doMouseRelease(Mouse::Button::Right); break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pViewport::construct() -> void {
  hwnd = CreateWindow(L"hiroViewport", L"",
    WS_CHILD | WS_DISABLED,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
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

}

#endif
