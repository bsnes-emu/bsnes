static HFONT Font_createFont(const string &family, unsigned size, bool bold, bool italic, bool underline) {
  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold == false ? FW_NORMAL : FW_BOLD, italic, underline, 0, 0, 0, 0, 0, 0,
    utf16_t(family)
  );
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
  hfont = 0;
}
