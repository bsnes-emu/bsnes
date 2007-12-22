void pButton::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindow("BUTTON", text ? text : "", WS_CHILD | WS_VISIBLE,
    0, 0, width, height,
    miu().p.default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)miu().p.default_font, 0);
}

void pButton::set_text(const char *text) {
  SetWindowText(hwnd, text ? text : "");
}

pButton::pButton(Button &self_) : pFormControl(self_), self(self_) {
}
