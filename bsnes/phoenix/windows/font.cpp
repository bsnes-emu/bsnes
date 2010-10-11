static HFONT Font_createFont(const string &name, unsigned size, bool bold, bool italic) {
  return CreateFont(
    -(size * 96.0 / 72.0 + 0.5),
    0, 0, 0, bold == false ? FW_NORMAL : FW_BOLD, italic, 0, 0, 0, 0, 0, 0, 0,
    utf16_t(name)
  );
}

bool Font::create(const string &name, unsigned size, Font::Style style) {
  font->font = Font_createFont(
    name, size,
    (style & Font::Style::Bold) == Font::Style::Bold,
    (style & Font::Style::Italic) == Font::Style::Italic
  );
  return font->font;
}

Font::Font() {
  font = new Font::Data;
  font->font = 0;
}

Font::~Font() {
  if(font->font) DeleteObject(font->font);
  delete font;
}
