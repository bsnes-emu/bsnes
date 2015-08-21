#if defined(Hiro_Font)

Font::Font() {
}

Font::Font(const string& family, unsigned size) {
  state.family = family;
  state.size = size;
}

Font::operator bool() const {
  return state.family || state.size || state.bold || state.italic;
}

auto Font::operator==(const Font& source) const -> bool {
  return family() == source.family() || size() == source.size() && bold() == source.bold() && italic() == source.italic();
};

auto Font::operator!=(const Font& source) const -> bool {
  return !operator==(source);
}

auto Font::bold() const -> bool {
  return state.bold;
}

auto Font::family() const -> string {
  return state.family;
}

auto Font::italic() const -> bool {
  return state.italic;
}

auto Font::setBold(bool bold) -> type& {
  state.bold = bold;
  return *this;
}

auto Font::setFamily(const string& family) -> type& {
  state.family = family;
  return *this;
}

auto Font::setItalic(bool italic) -> type& {
  state.italic = italic;
  return *this;
}

auto Font::setSize(unsigned size) -> type& {
  state.size = size;
  return *this;
}

auto Font::size() const -> unsigned {
  return state.size;
}

//

auto Font::serif(unsigned size, const string& style) -> string {
  return pFont::serif(size, style);
}

auto Font::sans(unsigned size, const string& style) -> string {
  return pFont::sans(size, style);
}

auto Font::monospace(unsigned size, const string& style) -> string {
  return pFont::monospace(size, style);
}

auto Font::size(const string& font, const string& text) -> Size {
  return pFont::size(font, text);
}

#endif
