static LRESULT CALLBACK Viewport_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

uintptr_t Viewport::handle() {
  return (uintptr_t)widget->window;
}

Viewport::Viewport() {
  widget->window = CreateWindow(
    L"phoenix_viewport", L"",
    WS_CHILD | WS_VISIBLE | WS_DISABLED,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}
