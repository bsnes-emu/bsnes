Geometry pVerticalScrollBar::minimumGeometry() {
  return { 0, 0, 18, 0 };
}

unsigned pVerticalScrollBar::position() {
  return GetScrollPos(hwnd, SB_CTL);
}

void pVerticalScrollBar::setLength(unsigned length) {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
  verticalScrollBar.setPosition(0);
}

void pVerticalScrollBar::setPosition(unsigned position) {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

void pVerticalScrollBar::constructor() {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | SBS_VERT,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&verticalScrollBar);
  unsigned position = verticalScrollBar.state.position;
  setLength(verticalScrollBar.state.length);
  verticalScrollBar.setPosition(position);
  synchronize();
}

void pVerticalScrollBar::destructor() {
  DestroyWindow(hwnd);
}

void pVerticalScrollBar::orphan() {
  destructor();
  constructor();
}
