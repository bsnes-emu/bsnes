Geometry pVerticalSlider::minimumGeometry() {
  return { 0, 0, 0, 25 };
}

unsigned pVerticalSlider::position() {
  return SendMessage(hwnd, TBM_GETPOS, 0, 0);
}

void pVerticalSlider::setLength(unsigned length) {
  length += (length == 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
  verticalSlider.setPosition(0);
}

void pVerticalSlider::setPosition(unsigned position) {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

void pVerticalSlider::constructor() {
  setParent(Window::None);
}

void pVerticalSlider::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindow(
    TRACKBAR_CLASS, L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH | TBS_HORZ,
    0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&verticalSlider);
  setLength(verticalSlider.state.length);
  setPosition(verticalSlider.state.position);
}
