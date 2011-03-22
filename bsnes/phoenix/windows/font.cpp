static HFONT Font_createFont(const string &family, unsigned size, bool bold, bool italic, bool underline) {
  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold == false ? FW_NORMAL : FW_BOLD, italic, underline, 0, 0, 0, 0, 0, 0,
    utf16_t(family)
  );
}

Geometry pFont::geometry(const string &text) {
  HDC hdc = GetDC(0);
  SelectObject(hdc, hfont);
  RECT rc = { 0, 0, 0, 0 };
  DrawText(hdc, utf16_t(text), -1, &rc, DT_CALCRECT);
  ReleaseDC(0, hdc);
  return { 0, 0, rc.right, rc.bottom };
}

void pFont::setBold(bool bold) {
  if(hfont) { DeleteObject(hfont); hfont = 0; }
  hfont = Font_createFont(font.state.family, font.state.size, font.state.bold, font.state.italic, font.state.underline);
}

void pFont::setFamily(const string &family) {
  if(hfont) { DeleteObject(hfont); hfont = 0; }
  hfont = Font_createFont(font.state.family, font.state.size, font.state.bold, font.state.italic, font.state.underline);
}

void pFont::setItalic(bool italic) {
  if(hfont) { DeleteObject(hfont); hfont = 0; }
  hfont = Font_createFont(font.state.family, font.state.size, font.state.bold, font.state.italic, font.state.underline);
}

void pFont::setSize(unsigned size) {
  if(hfont) { DeleteObject(hfont); hfont = 0; }
  hfont = Font_createFont(font.state.family, font.state.size, font.state.bold, font.state.italic, font.state.underline);
}

void pFont::setUnderline(bool underline) {
  if(hfont) { DeleteObject(hfont); hfont = 0; }
  hfont = Font_createFont(font.state.family, font.state.size, font.state.bold, font.state.italic, font.state.underline);
}

void pFont::constructor() {
  hfont = Font_createFont("Tahoma", 8, false, false, false);
}
