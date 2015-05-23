#if defined(Hiro_Font)

namespace hiro {

string pFont::serif(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Serif, ", size, ", ", style};
}

string pFont::sans(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Sans, ", size, ", ", style};
}

string pFont::monospace(unsigned size, string style) {
  if(size == 0) size = 8;
  return {"Liberation Mono, ", size, ", ", style};
}

Size pFont::size(string font, string text) {
  PangoFontDescription* description = create(font);
  Size size = pFont::size(description, text);
  free(description);
  return size;
}

PangoFontDescription* pFont::create(string description) {
  lstring part = description.split<2>(",").strip();

  string family = "Sans";
  unsigned size = 8u;
  bool bold = false;
  bool italic = false;

  if(part[0] != "") family = part[0];
  if(part.size() >= 2) size = decimal(part[1]);
  if(part.size() >= 3) bold = (bool)part[2].find("Bold");
  if(part.size() >= 3) italic = (bool)part[2].find("Italic");

  PangoFontDescription* font = pango_font_description_new();
  pango_font_description_set_family(font, family);
  pango_font_description_set_size(font, size * PANGO_SCALE);
  pango_font_description_set_weight(font, !bold ? PANGO_WEIGHT_NORMAL : PANGO_WEIGHT_BOLD);
  pango_font_description_set_style(font, !italic ? PANGO_STYLE_NORMAL : PANGO_STYLE_OBLIQUE);
  return font;
}

void pFont::free(PangoFontDescription* font) {
  pango_font_description_free(font);
}

Size pFont::size(PangoFontDescription* font, string text) {
  PangoContext* context = gdk_pango_context_get_for_screen(gdk_screen_get_default());
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_font_description(layout, font);
  pango_layout_set_text(layout, text, -1);
  int width = 0, height = 0;
  pango_layout_get_pixel_size(layout, &width, &height);
  g_object_unref((gpointer)layout);
  return {width, height};
}

void pFont::setFont(GtkWidget* widget, string font) {
  auto gtkFont = pFont::create(font);
  pFont::setFont(widget, (gpointer)gtkFont);
  pFont::free(gtkFont);
}

void pFont::setFont(GtkWidget* widget, gpointer font) {
  if(font == nullptr) return;
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)pFont::setFont, font);
  }
}

}

#endif
