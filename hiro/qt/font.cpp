#if defined(Hiro_Font)

namespace hiro {

auto pFont::size(const Font& font, const string& text) -> Size {
  return pFont::size(pFont::create(font), text);
}

auto pFont::size(const QFont& qtFont, const string& text) -> Size {
  QFontMetrics metrics(qtFont);
  signed maxWidth = 0;
  auto lines = text.split("\n");
  for(auto& line : lines) {
    maxWidth = max(maxWidth, metrics.width(QString::fromUtf8(line)));
  }
  return {maxWidth, metrics.height() * (signed)lines.size()};
}

auto pFont::family(const string& family) -> QString {
  if(family == Font::Sans ) return "Sans";
  if(family == Font::Serif) return "Serif";
  if(family == Font::Mono ) return "Liberation Mono";
  return family ? QString::fromUtf8(family) : "Sans";
}

auto pFont::create(const Font& font) -> QFont {
  QFont qtFont;
  qtFont.setFamily(family(font.family()));
  qtFont.setPointSize(font.size() ? font.size() : 8);
  qtFont.setBold(font.bold());
  qtFont.setItalic(font.italic());
  return qtFont;
}

}

#endif
