namespace phoenix {

Size pVerticalSlider::minimumSize() {
  return {0, 25};
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
  hwnd = CreateWindow(
    TRACKBAR_CLASS, L"", WS_CHILD | WS_TABSTOP | TBS_TRANSPARENTBKGND | TBS_NOTICKS | TBS_BOTH | TBS_VERT,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&verticalSlider);

  unsigned position = verticalSlider.state.position;
  setLength(verticalSlider.state.length);
  verticalSlider.setPosition(position);
  synchronize();
}

void pVerticalSlider::destructor() {
  DestroyWindow(hwnd);
}

void pVerticalSlider::orphan() {
  destructor();
  constructor();
}

void pVerticalSlider::onChange() {
  unsigned position = SendMessage(hwnd, TBM_GETPOS, 0, 0);
  if(position == verticalSlider.state.position) return;
  verticalSlider.state.position = position;
  if(verticalSlider.onChange) verticalSlider.onChange();
}

}
