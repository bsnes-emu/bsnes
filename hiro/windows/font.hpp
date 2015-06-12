#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto serif(unsigned size, string style) -> string;
  static auto sans(unsigned size, string style) -> string;
  static auto monospace(unsigned size, string style) -> string;
  static auto size(const string& font, const string& text) -> Size;

  static auto create(const string& description) -> HFONT;
  static auto free(HFONT hfont) -> void;
  static auto size(HFONT hfont, string text) -> Size;
};

}

#endif
