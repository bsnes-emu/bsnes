void Button::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindow(
    L"BUTTON", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : os.os->proportionalFont), 0);
}
