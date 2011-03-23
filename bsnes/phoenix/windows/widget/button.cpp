Geometry pButton::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(button.state.text);
  return { 0, 0, geometry.width + 20, font.p.height() + 12 };
}

void pButton::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pButton::constructor() {
  setParent(Window::None);
}

void pButton::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&button);
  setDefaultFont();
  setText(button.state.text);
}
