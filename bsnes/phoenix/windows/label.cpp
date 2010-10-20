void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindow(
    L"STATIC", L"",
    WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : os.os->proportionalFont), 0);
  setText(text);
}

void Label::setText(const char *text) {
  SetWindowText(widget->window, utf16_t(text));
}
