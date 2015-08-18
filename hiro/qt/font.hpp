#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto serif(unsigned size, string style) -> string;
  static auto sans(unsigned size, string style) -> string;
  static auto monospace(unsigned size, string style) -> string;
  static auto size(string font, string text) -> Size;

  static auto create(string description) -> QFont;
  static auto size(const QFont& qtFont, const string& text) -> Size;
};

}

#endif
