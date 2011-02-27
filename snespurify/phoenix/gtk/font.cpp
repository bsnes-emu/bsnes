void pFont::setBold(bool bold) {
  pango_font_description_set_weight(gtkFont, bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
}

void pFont::setFamily(const string &family) {
  pango_font_description_set_family(gtkFont, family);
}

void pFont::setItalic(bool italic) {
  pango_font_description_set_style(gtkFont, italic ? PANGO_STYLE_OBLIQUE : PANGO_STYLE_NORMAL);
}

void pFont::setSize(unsigned size) {
  pango_font_description_set_size(gtkFont, size * PANGO_SCALE);
}

void pFont::setUnderline(bool underline) {
}

void pFont::constructor() {
  gtkFont = pango_font_description_new();
}
