bool pCheckBox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

void pCheckBox::setChecked(bool checked) {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

void pCheckBox::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pCheckBox::constructor() {
  setParent(Window::None);
}

void pCheckBox::setParent(Window &parent) {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_CHECKBOX,
    0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&checkBox);
  setDefaultFont();
  if(checkBox.state.checked) setChecked(true);
  setText(checkBox.state.text);
}
