namespace phoenix {

bool pCheckButton::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

Size pCheckButton::minimumSize() {
  Size size = pFont::size(hfont, checkButton.state.text);
  return {size.width + 20, size.height + 4};
}

void pCheckButton::setChecked(bool checked) {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

void pCheckButton::setText(const string &text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pCheckButton::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX,
    0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&checkButton);
  setDefaultFont();
  if(checkButton.state.checked) setChecked(true);
  setText(checkButton.state.text);
  synchronize();

}

void pCheckButton::destructor() {
  DestroyWindow(hwnd);
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

}
