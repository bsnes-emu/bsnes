bool Font::create(const char *name, unsigned size, Font::Style style) {
  font->font = pango_font_description_new();
  pango_font_description_set_family(font->font, name);
  pango_font_description_set_absolute_size(font->font, size * PANGO_SCALE);
  pango_font_description_set_style(font->font, (style & Style::Italic) == Style::Italic ? PANGO_STYLE_OBLIQUE : PANGO_STYLE_NORMAL);
  pango_font_description_set_weight(font->font, (style & Style::Bold) == Style::Bold ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
  return true;
}

Font::Font() {
  font = new Font::Data;
  font->font = 0;
}

Font::~Font() {
  if(font->font) pango_font_description_free(font->font);
  delete font;
}
