namespace phoenix {

bool pRadioButton::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

Size pRadioButton::minimumSize() {
  Size size = pFont::size(hfont, radioButton.state.text);
  return {size.width + 20, size.height + 4};
}

void pRadioButton::setChecked() {
  for(auto& item : radioButton.state.group) {
    SendMessage(item.p.hwnd, BM_SETCHECK, (WPARAM)(&item == &radioButton), 0);
  }
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
}

void pRadioButton::setText(string text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pRadioButton::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&radioButton);
  setDefaultFont();
  if(radioButton.state.checked) setChecked();
  setText(radioButton.state.text);
  synchronize();
}

void pRadioButton::destructor() {
  DestroyWindow(hwnd);
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

}
