#if defined(Hiro_Font)

namespace hiro {

auto pFont::serif(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Serif, ", size, ", ", style};
}

auto pFont::sans(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Sans, ", size, ", ", style};
}

auto pFont::monospace(unsigned size, string style) -> string {
  if(size == 0) size = 8;
  return {"Liberation Mono, ", size, ", ", style};
}

auto pFont::size(string font, string text) -> Size {
  PangoFontDescription* description = create(font);
  Size size = pFont::size(description, text);
  free(description);
  return size;
}

auto pFont::create(string description) -> PangoFontDescription* {
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

auto pFont::free(PangoFontDescription* font) -> void {
  pango_font_description_free(font);
}

auto pFont::size(PangoFontDescription* font, string text) -> Size {
  PangoContext* context = gdk_pango_context_get_for_screen(gdk_screen_get_default());
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_font_description(layout, font);
  pango_layout_set_text(layout, text, -1);
  int width = 0, height = 0;
  pango_layout_get_pixel_size(layout, &width, &height);
  g_object_unref((gpointer)layout);
  return {width, height};
}

auto pFont::setFont(GtkWidget* widget, string font) -> void {
  auto gtkFont = pFont::create(font);
  pFont::setFont(widget, (gpointer)gtkFont);
  pFont::free(gtkFont);
}

auto pFont::setFont(GtkWidget* widget, gpointer font) -> void {
  if(font == nullptr) return;
  gtk_widget_modify_font(widget, (PangoFontDescription*)font);
  if(GTK_IS_CONTAINER(widget)) {
    gtk_container_foreach(GTK_CONTAINER(widget), (GtkCallback)pFont::setFont, font);
  }
}

}

#endif
