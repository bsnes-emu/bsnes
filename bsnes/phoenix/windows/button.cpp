void Button::setParent(Layout &layout) {
  SetParent(widget->window, layout.widget->window);
}

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
}

#if 0
void Button::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  widget->window = CreateWindow(
    L"BUTTON", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
}
#endif