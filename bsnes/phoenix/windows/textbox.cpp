string TextBox::text() {
  unsigned length = GetWindowTextLength(widget->window);
  wchar_t text[length + 1];
  GetWindowText(widget->window, text, length + 1);
  text[length] = 0;
  return utf8_t(text);
}

void TextBox::setText(const string &text) {
  object->locked = true;
  SetWindowText(widget->window, utf16_t(text));
  object->locked = false;
}

void TextBox::setEditable(bool editable) {
  SendMessage(widget->window, EM_SETREADONLY, editable == false, 0);
}

TextBox::TextBox() {
  widget->window = CreateWindowEx(
    WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}
