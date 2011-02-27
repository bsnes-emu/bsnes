void pFont::setBold(bool bold) { update(); }
void pFont::setFamily(const string &family) { update(); }
void pFont::setItalic(bool italic) { update(); }
void pFont::setSize(unsigned size) { update(); }
void pFont::setUnderline(bool underline) { update(); }

void pFont::constructor() {
  qtFont = new QFont;
}

void pFont::update() {
  qtFont->setFamily(QString::fromUtf8(font.state.family));
  qtFont->setPointSize(font.state.size);
  qtFont->setBold(font.state.bold);
  qtFont->setItalic(font.state.italic);
  qtFont->setUnderline(font.state.underline);
}
