namespace phoenix {

string pFont::serif(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Serif, ", size, ", ", style};
}

string pFont::sans(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Sans, ", size, ", ", style};
}

string pFont::monospace(unsigned size, string style) {
  if(size == 0) size = 8;
  if(style == "") style = "Normal";
  return {"Liberation Mono, ", size, ", ", style};
}

Size pFont::size(string font, string text) {
  return pFont::size(pFont::create(font), text);
}

QFont pFont::create(string description) {
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

Size pFont::size(const QFont& qtFont, string text) {
  QFontMetrics metrics(qtFont);

  lstring lines;
  lines.split("\n", text);

  unsigned maxWidth = 0;
  for(auto& line : lines) {
    maxWidth = max(maxWidth, metrics.width(line));
  }

  return {maxWidth, metrics.height() * lines.size()};
}

}
