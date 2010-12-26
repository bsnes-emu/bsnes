bool Font::create(const string &name, unsigned size, Font::Style style) {
  font->setFamily(QString::fromUtf8(name));
  font->setPointSize(size);
  font->setBold((style & Style::Bold) == Style::Bold);
  font->setItalic((style & Style::Italic) == Style::Italic);
}

Font::Font() {
  font = new Font::Data(*this);
}

Font::~Font() {
  delete font;
}
