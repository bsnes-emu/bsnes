namespace phoenix {

Position pMouse::position() {
  POINT point = {0};
  GetCursorPos(&point);
  return {point.x, point.y};
}

bool pMouse::pressed(Mouse::Button button) {
  switch(button) {
  case Mouse::Button::Left: return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
  case Mouse::Button::Middle: return GetAsyncKeyState(VK_MBUTTON) & 0x8000;
  case Mouse::Button::Right: return GetAsyncKeyState(VK_RBUTTON) & 0x8000;
  }
  return false;
}

}
