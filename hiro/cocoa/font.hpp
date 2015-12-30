#if defined(Hiro_Font)

namespace hiro {

struct pFont {
  static auto size(const Font& font, const string& text) -> Size;
  static auto size(NSFont* font, const string& text) -> Size;
  static auto family(const string& family) -> string;
  static auto create(const Font& font) -> NSFont*;
};

}

#endif
