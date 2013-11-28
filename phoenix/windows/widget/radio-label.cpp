namespace phoenix {

Size pRadioLabel::minimumSize() {
  Size size = pFont::size(hfont, radioLabel.state.text);
  return {size.width + 20, size.height + 4};
}

void pRadioLabel::setChecked() {
  for(auto& item : radioLabel.state.group) {
    SendMessage(item.p.hwnd, BM_SETCHECK, (WPARAM)(&item == &radioLabel), 0);
  }
}

void pRadioLabel::setGroup(const group<RadioLabel>& group) {
}

void pRadioLabel::setText(string text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pRadioLabel::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&radioLabel);
  setDefaultFont();
  if(radioLabel.state.checked) setChecked();
  setText(radioLabel.state.text);
  synchronize();
}

void pRadioLabel::destructor() {
  DestroyWindow(hwnd);
}

void pRadioLabel::orphan() {
  destructor();
  constructor();
}

void pRadioLabel::onActivate() {
  if(radioLabel.state.checked) return;
  radioLabel.setChecked();
  if(radioLabel.onActivate) radioLabel.onActivate();
}

}
