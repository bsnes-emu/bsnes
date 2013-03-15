namespace phoenix {

Size pVerticalScroller::minimumSize() {
  return {18, 0};
}

unsigned pVerticalScroller::position() {
  return GetScrollPos(hwnd, SB_CTL);
}

void pVerticalScroller::setLength(unsigned length) {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
  verticalScroller.setPosition(0);
}

void pVerticalScroller::setPosition(unsigned position) {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

void pVerticalScroller::constructor() {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | SBS_VERT,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&verticalScroller);
  unsigned position = verticalScroller.state.position;
  setLength(verticalScroller.state.length);
  verticalScroller.setPosition(position);
  synchronize();
}

void pVerticalScroller::destructor() {
  DestroyWindow(hwnd);
}

void pVerticalScroller::orphan() {
  destructor();
  constructor();
}

}
