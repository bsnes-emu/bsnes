namespace phoenix {

void pFrame::setEnabled(bool enabled) {
  if(frame.state.layout) frame.state.layout->setEnabled(frame.state.layout->enabled());
  pWidget::setEnabled(enabled);
}

void pFrame::setGeometry(Geometry geometry) {
  bool empty = frame.state.text.empty();
  Size size = pFont::size(hfont, frame.state.text);
  pWidget::setGeometry({
    geometry.x, geometry.y - (empty ? size.height >> 1 : 0),
    geometry.width, geometry.height + (empty ? size.height >> 1 : 0)
  });
  if(frame.state.layout == nullptr) return;
  if(empty) size.height = 1;
  geometry.x += 1, geometry.width -= 2;
  geometry.y += size.height, geometry.height -= size.height + 2;
  frame.state.layout->setGeometry(geometry);
}

void pFrame::setText(string text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pFrame::setVisible(bool visible) {
  if(frame.state.layout) frame.state.layout->setVisible(frame.state.layout->visible());
  pWidget::setVisible(visible);
}

void pFrame::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"",
    WS_CHILD | BS_GROUPBOX,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&frame);
  setDefaultFont();
  setText(frame.state.text);
  synchronize();
}

void pFrame::destructor() {
  DestroyWindow(hwnd);
}

void pFrame::orphan() {
  destructor();
  constructor();
}

}
