#if defined(Hiro_Mouse)

namespace hiro {

auto pMouse::position() -> Position {
  return {0, 0};
}

auto pMouse::pressed(Mouse::Button button) -> bool {
  return false;
}

}

#endif
