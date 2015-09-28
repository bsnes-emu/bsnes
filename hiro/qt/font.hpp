#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto size(const Font& font, const string& text) -> Size;
  static auto size(const QFont& qtFont, const string& text) -> Size;
  static auto family(const string& family) -> QString;
  static auto create(const Font& font) -> QFont;
};

}

#endif
