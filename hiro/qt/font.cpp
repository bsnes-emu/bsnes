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
  if(style == "") style = "Normal";
  return {"Liberation Mono, ", size, ", ", style};
}

auto pFont::size(string font, string text) -> Size {
  return pFont::size(pFont::create(font), text);
}

auto pFont::create(string description) -> QFont {
  lstring part = description.split(",", 2L).strip();

  string family = "Sans";
  unsigned size = 8u;
  bool bold = false;
  bool italic = false;

  if(part[0] != "") family = part[0];
  if(part.size() >= 2) size = decimal(part[1]);
  if(part.size() >= 3) bold = (bool)part[2].find("Bold");
  if(part.size() >= 3) italic = (bool)part[2].find("Italic");

  QFont qtFont;
  qtFont.setFamily(family);
  qtFont.setPointSize(size);
  if(bold) qtFont.setBold(true);
  if(italic) qtFont.setItalic(true);
  return qtFont;
}

auto pFont::size(const QFont& qtFont, const string& text) -> Size {
  QFontMetrics metrics(qtFont);

  lstring lines;
  lines.split(text ? text : " ", "\n");

  unsigned maxWidth = 0;
  for(auto& line : lines) {
    maxWidth = max(maxWidth, metrics.width(line));
  }

  return Size().setWidth(maxWidth).setHeight(metrics.height() * lines.size());
}

}

#endif
