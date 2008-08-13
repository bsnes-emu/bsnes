void pButton::create(unsigned style, unsigned width, unsigned height, const char *text) {
  hwnd = CreateWindow(L"BUTTON", utf16(text), WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
}

void pButton::set_text(const char *text) {
  SetWindowText(hwnd, utf16(text));
}

pButton::pButton(Button &self_) : pFormControl(self_), self(self_) {
}
