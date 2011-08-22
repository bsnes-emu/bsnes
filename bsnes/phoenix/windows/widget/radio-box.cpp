bool pRadioBox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

Geometry pRadioBox::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(radioBox.state.text);
  return { 0, 0, geometry.width + 20, font.p.height() + 4 };
}

void pRadioBox::setChecked() {
  foreach(item, radioBox.state.group) {
    SendMessage(item.p.hwnd, BM_SETCHECK, (WPARAM)(&item == &radioBox), 0);
  }
}

void pRadioBox::setGroup(const reference_array<RadioBox&> &group) {
}

void pRadioBox::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pRadioBox::constructor() {
  setParent(Window::None);
}

void pRadioBox::setParent(Window &parent) {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_RADIOBUTTON,
    0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&radioBox);
  setDefaultFont();
  if(radioBox.state.checked) setChecked();
  setText(radioBox.state.text);
  widget.setVisible(widget.visible());
}
