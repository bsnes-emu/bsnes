#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(string font, string text);

  static PangoFontDescription* create(string description);
  static void free(PangoFontDescription* font);
  static Size size(PangoFontDescription* font, string text);
  static void setFont(GtkWidget* widget, string font);
  static void setFont(GtkWidget* widget, gpointer font);
};

}

#endif
