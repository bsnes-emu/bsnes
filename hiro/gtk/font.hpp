#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto serif(unsigned size, string style) -> string;
  static auto sans(unsigned size, string style) -> string;
  static auto monospace(unsigned size, string style) -> string;
  static auto size(string font, string text) -> Size;

  static auto create(string description) -> PangoFontDescription*;
  static auto free(PangoFontDescription* font) -> void;
  static auto size(PangoFontDescription* font, string text) -> Size;
  static auto setFont(GtkWidget* widget, string font) -> void;
  static auto setFont(GtkWidget* widget, gpointer font) -> void;
};

}

#endif
