#if defined(Hiro_Label)

auto mLabel::allocate() -> pObject* {
  return new pLabel(*this);
}

//

auto mLabel::horizontalAlignment() const -> double {
  return state.horizontalAlignment;
}

auto mLabel::setHorizontalAlignment(double alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.horizontalAlignment = alignment;
  signal(setHorizontalAlignment, alignment);
  return *this;
}

auto mLabel::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mLabel::setVerticalAlignment(double alignment) -> type& {
  alignment = max(0.0, min(1.0, alignment));
  state.verticalAlignment = alignment;
  signal(setVerticalAlignment, alignment);
  return *this;
}

auto mLabel::text() const -> string {
  return state.text;
}

auto mLabel::verticalAlignment() const -> double {
  return state.verticalAlignment;
}

#endif
