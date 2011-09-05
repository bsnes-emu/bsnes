Geometry pButton::minimumGeometry() {
  Geometry geometry = pFont::geometry(hfont, button.state.text);
  return { 0, 0, geometry.width + 20, geometry.height + 10 };
}

void pButton::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pButton::constructor() {
  hwnd = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&button);
  setDefaultFont();
  setText(button.state.text);
  synchronize();
}

void pButton::destructor() {
  DestroyWindow(hwnd);
}

void pButton::orphan() {
  destructor();
  constructor();
}
