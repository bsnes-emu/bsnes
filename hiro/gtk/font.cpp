#if defined(Hiro_Font)

namespace hiro {

auto pFont::size(const Font& font, const string& text) -> Size {
  PangoFontDescription* description = create(font);
  Size size = pFont::size(description, text);
  free(description);
  return size;
}

auto pFont::size(PangoFontDescription* font, const string& text) -> Size {
  PangoContext* context = gdk_pango_context_get_for_screen(gdk_screen_get_default());
  PangoLayout* layout = pango_layout_new(context);
  pango_layout_set_font_description(layout, font);
  pango_layout_set_text(layout, text, -1);
  signed width = 0, height = 0;
  pango_layout_get_pixel_size(layout, &width, &height);
  g_object_unref((gpointer)layout);
  return {width, height};
}

auto pFont::family(const string& family) -> string {
  #if defined(DISPLAY_WINDOWS)
  if(family == Font::Sans ) return "Tahoma";
  if(family == Font::Serif) return "Georgia";
  if(family == Font::Mono ) return "Lucida Console";
  return family ? family : "Tahoma";
  #elif defined(DISPLAY_XORG)
  if(family == Font::Sans ) return "Sans";
  if(family == Font::Serif) return "Serif";
  if(family == Font::Mono ) return "Liberation Mono";
  return family ? family : "Sans";
  #else
  return family;
  #endif
}

auto pFont::create(const Font& font) -> PangoFontDescription* {
  auto p = pango_font_description_new();
  pango_font_description_set_family(p, family(font.family()));
  pango_font_description_set_size(p, (font.size() ? font.size() : 8) * PANGO_SCALE);
  pango_font_description_set_weight(p, font.bold() ? PANGO_WEIGHT_BOLD : PANGO_WEIGHT_NORMAL);
  pango_font_description_set_style(p, font.italic() ? PANGO_STYLE_OBLIQUE : PANGO_STYLE_NORMAL);
  return p;
}

auto pFont::free(PangoFontDescription* font) -> void {
  pango_font_description_free(font);
}

auto pFont::setFont(GtkWidget* widget, const Font& font) -> void {
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
