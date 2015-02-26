namespace phoenix {

Size pCheckLabel::minimumSize() {
  Size size = pFont::size(hfont, checkLabel.state.text);
  return {size.width + 20, size.height + 4};
}

void pCheckLabel::setChecked(bool checked) {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
}

void pCheckLabel::setText(string text) {
  SetWindowText(hwnd, utf16_t(text));
}

void pCheckLabel::constructor() {
  hwnd = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | BS_CHECKBOX,
    0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&checkLabel);
  setDefaultFont();
  setChecked(checkLabel.state.checked);
  setText(checkLabel.state.text);
  synchronize();
}

void pCheckLabel::destructor() {
  DestroyWindow(hwnd);
}

void pCheckLabel::orphan() {
  destructor();
  constructor();
}

void pCheckLabel::onToggle() {
  checkLabel.state.checked = !checkLabel.state.checked;
  setChecked(checkLabel.state.checked);
  if(checkLabel.onToggle) checkLabel.onToggle();
}

}
