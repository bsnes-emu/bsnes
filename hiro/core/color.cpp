#if defined(Hiro_Color)

Color::Color() {
  setColor(0, 0, 0, 0);
}

Color::Color(signed red, signed green, signed blue, signed alpha) {
  setColor(red, green, blue, alpha);
}

Color::operator bool() const {
  return state.red || state.green || state.blue || state.alpha;
}

auto Color::operator==(const Color& source) const -> bool {
  return red() == source.red() && green() == source.green() && blue() == source.blue() && alpha() == source.alpha();
}

auto Color::operator!=(const Color& source) const -> bool {
  return !operator==(source);
}

auto Color::alpha() const -> uint8_t {
  return state.alpha;
}

auto Color::blue() const -> uint8_t {
  return state.blue;
}

auto Color::green() const -> uint8_t {
  return state.green;
}

auto Color::red() const -> uint8_t {
  return state.red;
}

auto Color::reset() -> type& {
  return setColor(0, 0, 0, 0);
}

auto Color::setAlpha(signed alpha) -> type& {
  state.alpha = max(0, min(255, alpha));
  return *this;
}

auto Color::setBlue(signed blue) -> type& {
  state.blue = max(0, min(255, blue));
  return *this;
}

auto Color::setColor(Color color) -> type& {
  return setColor(color.red(), color.green(), color.blue(), color.alpha());
}

auto Color::setColor(signed red, signed green, signed blue, signed alpha) -> type& {
  state.red   = max(0, min(255, red  ));
  state.green = max(0, min(255, green));
  state.blue  = max(0, min(255, blue ));
  state.alpha = max(0, min(255, alpha));
  return *this;
}

auto Color::setGreen(signed green) -> type& {
  state.green = max(0, min(255, green));
  return *this;
}

auto Color::setRed(signed red) -> type& {
  state.red = max(0, min(255, red));
  return *this;
}

auto Color::value() const -> uint32_t {
  return state.alpha << 24 | state.red << 16 | state.green << 8 | state.blue << 0;
}

#endif
