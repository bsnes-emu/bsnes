void Button::setText(const string &text) {
  SetWindowText(widget->window, utf16_t(text));
}

Button::Button() {
  widget->window = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}
