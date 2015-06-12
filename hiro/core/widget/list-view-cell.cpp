#if defined(Hiro_ListView)

auto mListViewCell::allocate() -> pObject* {
  return new pListViewCell(*this);
}

//

auto mListViewCell::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mListViewCell::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mListViewCell::icon() const -> image {
  return state.icon;
}

auto mListViewCell::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListViewCell::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListViewCell::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mListViewCell::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mListViewCell::text() const -> string {
  return state.text;
}

#endif
