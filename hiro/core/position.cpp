#if defined(Hiro_Position)

Position::Position() {
  setPosition(0, 0);
}

Position::Position(float x, float y) {
  setPosition(x, y);
}

Position::operator bool() const {
  return state.x || state.y;
}

auto Position::operator==(const Position& source) const -> bool {
  return x() == source.x() && y() == source.y();
}

auto Position::operator!=(const Position& source) const -> bool {
  return !operator==(source);
}

auto Position::reset() -> type& {
  return setPosition(0, 0);
}

auto Position::setPosition(Position position) -> type& {
  return setPosition(position.x(), position.y());
}

auto Position::setPosition(float x, float y) -> type& {
  state.x = x;
  state.y = y;
  return *this;
}

auto Position::setX(float x) -> type& {
  state.x = x;
  return *this;
}

auto Position::setY(float y) -> type& {
  state.y = y;
  return *this;
}

auto Position::x() const -> float {
  return state.x;
}

auto Position::y() const -> float {
  return state.y;
}

#endif
