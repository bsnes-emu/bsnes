uintptr_t pViewport::handle() {
  return (uintptr_t)hwnd;
}

void pViewport::constructor() {
  setParent(Window::None);
}

void pViewport::setParent(Window &parent) {
  hwnd = CreateWindow(L"phoenix_viewport", L"", WS_CHILD | WS_VISIBLE | WS_DISABLED, 0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&viewport);
  widget.setVisible(widget.visible());
}

static LRESULT CALLBACK Viewport_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}
