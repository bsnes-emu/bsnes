#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto size(const Font& font, const string& text) -> Size;
  static auto size(PangoFontDescription* font, const string& text) -> Size;
  static auto family(const string& family) -> string;
  static auto create(const Font& font) -> PangoFontDescription*;
  static auto free(PangoFontDescription* font) -> void;
  static auto setFont(GtkWidget* widget, const Font& font) -> void;
  static auto setFont(GtkWidget* widget, gpointer font) -> void;
};

}

#endif
