auto Mouse::position() -> Position {
  return pMouse::position();
}

auto Mouse::pressed(Mouse::Button button) -> bool {
  return pMouse::pressed(button);
}

auto Mouse::released(Mouse::Button button) -> bool {
  return !pressed(button);
}
