Geometry pFont::geometry(const string &description, const string &text) {
  PangoFontDescription *font = create(description);
  Geometry geometry = pFont::geometry(font, text);
  free(font);
  return geometry;
}

PangoFontDescription* pFont::create(const string &description) {
  lstring part;
  part.split(",", description);
  for(auto &item : part) item.trim(" ");

  string family = part[0];
  unsigned size = decimal(part[1]);
  bool bold = part[2].position("Bold");
  bool italic = part[2].position("Italic");
  if(family == "") family = "Sans";
  if(size == 0) size = 8;

  PangoFontDescription *font = pango_font_description_new();
  pango_font_description_set_family(font, family);
  pango_font_description_set_size(font, size * PANGO_SCALE);
  pango_font_description_set_weight(font, !bold ? PANGO_WEIGHT_NORMAL : PANGO_WEIGHT_BOLD);
  pango_font_description_set_style(font, !italic ? PANGO_STYLE_NORMAL : PANGO_STYLE_OBLIQUE);
  return font;
}

void pFont::free(PangoFontDescription *font) {
  pango_font_description_free(font);
}

Geometry pFont::geometry(PangoFontDescription *font, const string &text) {
  PangoContext *context = gdk_pango_context_get_for_screen(gdk_screen_get_default());
  PangoLayout *layout = pango_layout_new(context);
  pango_layout_set_font_description(layout, font);
  pango_layout_set_text(layout, text, -1);
  int width = 0, height = 0;
  pango_layout_get_pixel_size(layout, &width, &height);
  g_object_unref((gpointer)layout);
  return { 0, 0, width, height };
}

void pFont::setFont(GtkWidget *widget, const string &font) {
  auto gtkFont = pFont::create(font);
  pFont::setFont(widget, (gpointer)gtkFont);
  pFont::free(gtkFont);
}

void pFont::setFont(GtkWidget *widget, gpointer font) {
  if(font == 0) return;
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)pFont::setFont, font);
  }
}
