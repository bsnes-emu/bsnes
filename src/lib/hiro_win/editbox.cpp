void pEditbox::create(uint style, uint width, uint height, const char *text) {
  bool multiline = style & Editbox::Multiline;
  bool readonly = style & Editbox::Readonly;
  uint vscroll = (style & Editbox::VerticalScrollAlways) ? WS_VSCROLL :
                 (style & Editbox::VerticalScrollNever) ? 0 :
                 ES_AUTOVSCROLL;
  uint hscroll = (style & Editbox::HorizontalScrollAlways) ? WS_HSCROLL :
                 (style & Editbox::HorizontalScrollNever) ? 0 :
                 ES_AUTOHSCROLL;

  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
    WS_CHILD | WS_VISIBLE | vscroll | hscroll |
    (multiline == true ? ES_MULTILINE : 0) |
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
  SetWindowText(hwnd, temp);
}

uint pEditbox::get_text(char *text, uint length) {
  GetWindowText(hwnd, text, length);
  string temp = text;
  replace(temp, "\r", "");
  strcpy(text, temp);
  return strlen(text);
}

pEditbox::pEditbox(Editbox &self_) : pFormControl(self_), self(self_) {
}
