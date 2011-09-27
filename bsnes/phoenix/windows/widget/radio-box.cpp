bool pRadioBox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

Geometry pRadioBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(hfont, radioBox.state.text);
  return { 0, 0, geometry.width + 20, geometry.height + 4 };
}

void pRadioBox::setChecked() {
  for(auto &item : radioBox.state.group) {
    SendMessage(item.p.hwnd, BM_SETCHECK, (WPARAM)(&item == &radioBox), 0);
  }
}

void pRadioBox::setGroup(const reference_array<RadioBox&> &group) {
}

void pRadioBox::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pRadioBox::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&radioBox);
  setDefaultFont();
  if(radioBox.state.checked) setChecked();
  setText(radioBox.state.text);
  synchronize();
}

void pRadioBox::destructor() {
  DestroyWindow(hwnd);
}

void pRadioBox::orphan() {
  destructor();
  constructor();
}
