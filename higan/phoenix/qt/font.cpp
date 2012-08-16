Geometry pFont::geometry(const string &description, const string &text) {
  return pFont::geometry(pFont::create(description), text);
}

QFont pFont::create(const string &description) {
  lstring part;
  part.split(",", description);
  for(auto &item : part) item.trim(" ");

  string family = "Sans";
  unsigned size = 8u;
  bool bold = false;
  bool italic = false;

  if(part[0] != "") family = part[0];
  if(part.size() >= 2) size = decimal(part[1]);
  if(part.size() >= 3) bold = part[2].position("Bold");
  if(part.size() >= 3) italic = part[2].position("Italic");

  QFont qtFont;
  qtFont.setFamily(family);
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
