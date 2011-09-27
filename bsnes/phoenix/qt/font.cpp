Geometry pFont::geometry(const string &description, const string &text) {
  return pFont::geometry(pFont::create(description), text);
}

QFont pFont::create(const string &description) {
  lstring part;
  part.split(",", description);
  for(auto &item : part) item.trim(" ");

  string name = part[0] != "" ? part[0] : "Sans";
  unsigned size = part.size() >= 2 ? decimal(part[1]) : 8u;
  bool bold = part[2].position("Bold");
  bool italic = part[2].position("Italic");

  QFont qtFont;
  qtFont.setFamily(name);
  qtFont.setPointSize(size);
  if(bold) qtFont.setBold(true);
  if(italic) qtFont.setItalic(true);
  return qtFont;
}

Geometry pFont::geometry(const QFont &qtFont, const string &text) {
  QFontMetrics metrics(qtFont);

  lstring lines;
  lines.split("\n", text);

  unsigned maxWidth = 0;
  for(auto &line : lines) {
    maxWidth = max(maxWidth, metrics.width(line));
  }

  return { 0, 0, maxWidth, metrics.height() * lines.size() };
}
