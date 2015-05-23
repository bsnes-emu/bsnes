#if defined(Hiro_Position)

Position::Position() {
  setPosition(0, 0);
}

Position::Position(signed x, signed y) {
  setPosition(x, y);
}

auto Position::operator==(const Position& source) const -> bool {
  return x() == source.x() && y() == source.y();
}

auto Position::operator!=(const Position& source) const -> bool {
  return !operator==(source);
}

auto Position::setPosition(Position position) -> type& {
  return setPosition(position.x(), position.y());
}

auto Position::setPosition(signed x, signed y) -> type& {
  state.x = x;
  state.y = y;
  return *this;
}

auto Position::setX(signed x) -> type& {
  state.x = x;
  return *this;
}

auto Position::setY(signed y) -> type& {
  state.y = y;
  return *this;
}

auto Position::x() const -> signed {
  return state.x;
}

auto Position::y() const -> signed {
  return state.y;
}

#endif
