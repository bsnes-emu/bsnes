void TextBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
}

string TextBox::text() {
  unsigned length = GetWindowTextLength(widget->window);
  wchar_t text[length + 1];
  GetWindowText(widget->window, text, length + 1);
  text[length] = 0;
  return utf8_t(text);
}

void TextBox::setText(const char *text) {
  object->locked = true;
  SetWindowText(widget->window, utf16_t(text));
  object->locked = false;
}

void TextBox::setEditable(bool editable) {
  SendMessage(widget->window, EM_SETREADONLY, editable == false, 0);
}
