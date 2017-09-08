#if defined(Hiro_Alignment)

Alignment::Alignment() {
  setAlignment(-1.0, -1.0);
}

Alignment::Alignment(float horizontal, float vertical) {
  setAlignment(horizontal, vertical);
}

Alignment::operator bool() const {
  return state.horizontal >= 0.0 && state.horizontal <= 1.0
      && state.vertical   >= 0.0 && state.vertical   <= 1.0;
}

auto Alignment::horizontal() const -> float {
  return state.horizontal;
}

auto Alignment::reset() -> type& {
  return setAlignment(-1.0, -1.0);
}

auto Alignment::setAlignment(float horizontal, float vertical) -> type& {
  state.horizontal = horizontal;
  state.vertical   = vertical;
  return *this;
}

auto Alignment::setHorizontal(float horizontal) -> type& {
  state.horizontal = horizontal;
  return *this;
}

auto Alignment::setVertical(float vertical) -> type& {
  state.vertical = vertical;
  return *this;
}

auto Alignment::vertical() const -> float {
  return state.vertical;
}

#endif
