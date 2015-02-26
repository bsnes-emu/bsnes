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
