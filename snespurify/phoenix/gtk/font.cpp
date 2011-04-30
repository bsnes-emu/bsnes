Geometry pFont::geometry(const string &text) {
  pango_layout_set_font_description(gtkLayout, gtkFont);
  pango_layout_set_text(gtkLayout, text, -1);
  int width = 0, height = 0;
  pango_layout_get_pixel_size(gtkLayout, &width, &height);
  return { 0, 0, width, height };
}

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
  PangoContext *context = gdk_pango_context_get_for_screen(gdk_screen_get_default());
  gtkLayout = pango_layout_new(context);
}
