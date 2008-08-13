void pEditbox::create(unsigned style, unsigned width, unsigned height, const char *text) {
  bool multiline = style & Editbox::Multiline;
  bool readonly  = style & Editbox::Readonly;
  unsigned vscroll = (style & Editbox::VerticalScrollAlways) ? WS_VSCROLL :
                     (style & Editbox::VerticalScrollNever) ? 0 :
                     ES_AUTOVSCROLL;
  unsigned hscroll = (style & Editbox::HorizontalScrollAlways) ? WS_HSCROLL :
                     (style & Editbox::HorizontalScrollNever) ? 0 :
                     ES_AUTOHSCROLL;

  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_VISIBLE | vscroll | hscroll |
    (multiline == true ? ES_MULTILINE | ES_WANTRETURN : WS_TABSTOP) |
    (readonly == true ? ES_READONLY : 0),
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
  set_text(text);
}

void pEditbox::set_text(const char *text) {
  string temp = text ? text : "";
  replace(temp, "\r", "");
  replace(temp, "\n", "\r\n");
  SetWindowText(hwnd, utf16(temp));
}

unsigned pEditbox::get_text(char *text, unsigned length) {
  wchar_t *buffer = new wchar_t[length + 1];
  GetWindowText(hwnd, buffer, length);
  string temp = (const char*)utf8(buffer);
  delete[] buffer;
  replace(temp, "\r", "");
  strlcpy(text, temp, length);
  return strlen(text);
}

pEditbox::pEditbox(Editbox &self_) : pFormControl(self_), self(self_) {
}
