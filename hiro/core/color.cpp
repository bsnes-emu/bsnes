#if defined(Hiro_Color)

Color::Color() {
  setColor(0, 0, 0, 0);
}

Color::Color(signed red, signed green, signed blue) {
  setColor(255, red, green, blue);
}

Color::Color(signed alpha, signed red, signed green, signed blue) {
  setColor(alpha, red, green, blue);
}

Color::operator bool() const {
  return !empty();
}

auto Color::operator==(const Color& source) const -> bool {
  return alpha() == source.alpha() && red() == source.red() && green() == source.green() && blue() == source.blue();
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

auto Color::empty() const -> bool {
  return state.alpha == 0 && state.red == 0 && state.green == 0 && state.blue == 0;
}

auto Color::green() const -> uint8_t {
  return state.green;
}

auto Color::red() const -> uint8_t {
  return state.red;
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
  return setColor(color.alpha(), color.red(), color.green(), color.blue());
}

auto Color::setColor(signed red, signed green, signed blue) -> type& {
  return setColor(255, red, green, blue);
}

auto Color::setColor(signed alpha, signed red, signed green, signed blue) -> type& {
  state.alpha = max(0, min(255, alpha));
  state.red   = max(0, min(255, red  ));
  state.green = max(0, min(255, green));
  state.blue  = max(0, min(255, blue ));
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
  return (state.alpha << 24) + (state.red << 16) + (state.green << 8) + (state.blue << 0);
}

#endif
