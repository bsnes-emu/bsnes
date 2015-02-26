namespace hiro {

auto pMouse::position() -> Position {
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pApplication::display, DefaultRootWindow(pApplication::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  return {rootx, rooty};
}

auto pMouse::pressed(Mouse::Button button) -> bool {
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pApplication::display, DefaultRootWindow(pApplication::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  switch(button) {
  case Mouse::Button::Left: return mask & Button1Mask;
  case Mouse::Button::Middle: return mask & Button2Mask;
  case Mouse::Button::Right: return mask & Button3Mask;
  }
  return false;
}

}
