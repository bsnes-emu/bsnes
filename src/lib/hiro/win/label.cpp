void pLabel::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindow("STATIC", text ? text : "", WS_CHILD | WS_VISIBLE,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
}

void pLabel::set_text(const char *text) {
  SetWindowText(hwnd, text ? text : "");
}

pLabel::pLabel(Label &self_) : pFormControl(self_), self(self_) {
}
