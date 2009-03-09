void pEditbox::create(unsigned style, unsigned width, unsigned height, const char *text) {
  bool multiline = style & Editbox::Multiline;
  bool readonly  = style & Editbox::Readonly;
  unsigned vscroll = (style & Editbox::VerticalScrollAlways) ? WS_VSCROLL :
                     (style & Editbox::VerticalScrollNever ) ? 0 :
                     ES_AUTOVSCROLL;
  unsigned hscroll = (style & Editbox::HorizontalScrollAlways) ? WS_HSCROLL :
                     (style & Editbox::HorizontalScrollNever ) ? 0 :
                     ES_AUTOHSCROLL;

  autovscroll = (vscroll == ES_AUTOVSCROLL);

  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
    WS_CHILD | WS_VISIBLE | vscroll | hscroll |
    (multiline == true ? ES_MULTILINE | ES_WANTRETURN : WS_TABSTOP) |
    (readonly == true ? ES_READONLY : 0),
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
  set_text(text);
  update();
}

void pEditbox::resize(unsigned width, unsigned height) {
  pFormControl::resize(width, height);
  update();
}

void pEditbox::set_text(const char *text) {
  string temp = text ? text : "";
  temp.replace("\r", "");
  temp.replace("\n", "\r\n");
  SetWindowText(hwnd, utf16(temp));
  update();
}

unsigned pEditbox::get_text(char *text, unsigned length) {
  wchar_t *buffer = new wchar_t[length + 1];
  GetWindowText(hwnd, buffer, length);
  string temp = (const char*)utf8(buffer);
  delete[] buffer;
  temp.replace("\r", "");
  strlcpy(text, temp, length);
  return strlen(text);
}

pEditbox::pEditbox(Editbox &self_) : pFormControl(self_), self(self_) {
  autovscroll = false;
}

//========
//internal
//========

//WS_VSCROLL always shows the scrollbar;
//ES_AUTOVSCROLL never shows the scrollbar but allows unlimited text;
//no style disallows more text than what fits in the window.
//
//below routine simulates the effect of allowing unlimited text, but only
//showing the scrollbar when it is needed. not sure how to simulate this effect
//for horizontal scrolling at this time.
//
//below routine should be called whenever the font, text or window size is
//modified.
void pEditbox::update() {
  if(autovscroll == true) {
    //determine how many lines of text this control allows
    RECT rect;
    SendMessage(hwnd, EM_GETRECT, 0, (LPARAM)&rect);
    unsigned height = rect.bottom - rect.top;

    //determine the height of a single line of text
    HDC hdc = GetDC(hwnd);
    SelectObject(hdc, phiro().default_font);
    DrawText(hdc, L"byuu", -1, &rect, DT_CALCRECT);
    ReleaseDC(hwnd, hdc);
    unsigned lineheight = rect.bottom - rect.top;

    //only show the scrollbar when there are more lines of text
    //than the control can show at once.
    unsigned linecount = SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
    ShowScrollBar(hwnd, SB_VERT, linecount > (height / lineheight));
  }
}
