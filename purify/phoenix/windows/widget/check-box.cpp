bool pCheckBox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

Geometry pCheckBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(hfont, checkBox.state.text);
  return { 0, 0, geometry.width + 20, geometry.height + 4 };
}

void pCheckBox::setChecked(bool checked) {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

void pCheckBox::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pCheckBox::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&checkBox);
  setDefaultFont();
  if(checkBox.state.checked) setChecked(true);
  setText(checkBox.state.text);
  synchronize();

}

void pCheckBox::destructor() {
  DestroyWindow(hwnd);
}

void pCheckBox::orphan() {
  destructor();
  constructor();
}
