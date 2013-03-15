namespace phoenix {

Size pHorizontalScroller::minimumSize() {
  return {0, 18};
}

unsigned pHorizontalScroller::position() {
  return GetScrollPos(hwnd, SB_CTL);
}

void pHorizontalScroller::setLength(unsigned length) {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
  horizontalScroller.setPosition(0);
}

void pHorizontalScroller::setPosition(unsigned position) {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

void pHorizontalScroller::constructor() {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | WS_TABSTOP | SBS_HORZ,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&horizontalScroller);
  unsigned position = horizontalScroller.state.position;
  setLength(horizontalScroller.state.length);
  horizontalScroller.setPosition(position);
  synchronize();
}

void pHorizontalScroller::destructor() {
  DestroyWindow(hwnd);
}

void pHorizontalScroller::orphan() {
  destructor();
  constructor();
}

}
