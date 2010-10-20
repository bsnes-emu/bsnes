void TextBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : os.os->proportionalFont), 0);
}

void TextBox::setEditable(bool editable) {
  SendMessage(widget->window, EM_SETREADONLY, editable == false, (LPARAM)0);
}
