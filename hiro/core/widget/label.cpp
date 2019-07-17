#if defined(Hiro_Label)

auto mLabel::allocate() -> pObject* {
  return new pLabel(*this);
}

//

auto mLabel::alignment() const -> Alignment {
  return state.alignment;
}

auto mLabel::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mLabel::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mLabel::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mLabel::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mLabel::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mLabel::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mLabel::text() const -> string {
  return state.text;
}

#endif
