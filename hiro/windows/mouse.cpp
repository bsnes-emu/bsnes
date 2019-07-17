#if defined(Hiro_Mouse)

namespace hiro {

auto pMouse::position() -> Position {
  POINT point{};
  GetCursorPos(&point);
  return {point.x, point.y};
}

auto pMouse::pressed(Mouse::Button button) -> bool {
  switch(button) {
  case Mouse::Button::Left: return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
  case Mouse::Button::Middle: return GetAsyncKeyState(VK_MBUTTON) & 0x8000;
  case Mouse::Button::Right: return GetAsyncKeyState(VK_RBUTTON) & 0x8000;
  }
  return false;
}

}

#endif
