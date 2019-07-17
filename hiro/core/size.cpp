#if defined(Hiro_Size)

Size::Size() {
  setSize(0, 0);
}

Size::Size(float width, float height) {
  setSize(width, height);
}

Size::operator bool() const {
  return state.width || state.height;
}

auto Size::operator==(const Size& source) const -> bool {
  return width() == source.width() && height() == source.height();
}

auto Size::operator!=(const Size& source) const -> bool {
  return !operator==(source);
}

auto Size::height() const -> float {
  return state.height;
}

auto Size::reset() -> type& {
  return setSize(0, 0);
}

auto Size::setHeight(float height) -> type& {
  state.height = height;
  return *this;
}

auto Size::setSize(Size size) -> type& {
  return setSize(size.width(), size.height());
}

auto Size::setSize(float width, float height) -> type& {
  state.width = width;
  state.height = height;
  return *this;
}

auto Size::setWidth(float width) -> type& {
  state.width = width;
  return *this;
}

auto Size::width() const -> float {
  return state.width;
}

#endif
