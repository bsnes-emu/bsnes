#if defined(Hiro_Mouse)

namespace hiro {

auto pMouse::position() -> Position {
  #if defined(DISPLAY_WINDOWS)
  POINT point{0};
  GetCursorPos(&point);
  return {point.x, point.y};
  #endif

  #if defined(DISPLAY_XORG)
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pApplication::display, DefaultRootWindow(pApplication::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  return {rootx, rooty};
  #endif
}

auto pMouse::pressed(Mouse::Button button) -> bool {
  #if defined(DISPLAY_WINDOWS)
  switch(button) {
  case Mouse::Button::Left: return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
  case Mouse::Button::Middle: return GetAsyncKeyState(VK_MBUTTON) & 0x8000;
  case Mouse::Button::Right: return GetAsyncKeyState(VK_RBUTTON) & 0x8000;
  }
  #endif

  #if defined(DISPLAY_XORG)
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pApplication::display, DefaultRootWindow(pApplication::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  switch(button) {
  case Mouse::Button::Left: return mask & Button1Mask;
  case Mouse::Button::Middle: return mask & Button2Mask;
  case Mouse::Button::Right: return mask & Button3Mask;
  }
  #endif

  return false;
}

}

#endif
