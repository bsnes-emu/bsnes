namespace phoenix {

Size pHorizontalSlider::minimumSize() {
  return {0, 25};
}

void pHorizontalSlider::setLength(unsigned length) {
  length += (length == 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
  horizontalSlider.setPosition(0);
}

void pHorizontalSlider::setPosition(unsigned position) {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

void pHorizontalSlider::constructor() {
  hwnd = CreateWindow(
    TRACKBAR_CLASS, L"", WS_CHILD | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH | TBS_HORZ,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&horizontalSlider);
  unsigned position = horizontalSlider.state.position;
  setLength(horizontalSlider.state.length);
  horizontalSlider.setPosition(position);
  synchronize();
}

void pHorizontalSlider::destructor() {
  DestroyWindow(hwnd);
}

void pHorizontalSlider::orphan() {
  destructor();
  constructor();
}

void pHorizontalSlider::onChange() {
  unsigned position = SendMessage(hwnd, TBM_GETPOS, 0, 0);
  if(position == horizontalSlider.state.position) return;
  horizontalSlider.state.position = position;
  if(horizontalSlider.onChange) horizontalSlider.onChange();
}

}
