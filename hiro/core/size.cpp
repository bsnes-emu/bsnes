#if defined(Hiro_Size)

Size::Size() {
  setSize(0, 0);
}

Size::Size(signed width, signed height) {
  setSize(width, height);
}

auto Size::operator==(const Size& source) const -> bool {
  return width() == source.width() && height() == source.height();
}

auto Size::operator!=(const Size& source) const -> bool {
  return !operator==(source);
}

auto Size::height() const -> signed {
  return state.height;
}

auto Size::setHeight(signed height) -> type& {
  state.height = height;
  return *this;
}

auto Size::setSize(Size size) -> type& {
  return setSize(size.width(), size.height());
}

auto Size::setSize(signed width, signed height) -> type& {
  state.width = width;
  state.height = height;
  return *this;
}

auto Size::setWidth(signed width) -> type& {
  state.width = width;
  return *this;
}

auto Size::width() const -> signed {
  return state.width;
}

#endif
