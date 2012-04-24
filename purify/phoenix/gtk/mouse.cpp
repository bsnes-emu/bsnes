Position pMouse::position() {
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pOS::display, DefaultRootWindow(pOS::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  return { rootx, rooty };
}

bool pMouse::pressed(Mouse::Button button) {
  XlibWindow root, child;
  int rootx, rooty, winx, winy;
  unsigned int mask;
  XQueryPointer(pOS::display, DefaultRootWindow(pOS::display), &root, &child, &rootx, &rooty, &winx, &winy, &mask);
  switch(button) {
  case Mouse::Button::Left: return mask & Button1Mask;
  case Mouse::Button::Middle: return mask & Button2Mask;
  case Mouse::Button::Right: return mask & Button3Mask;
  }
  return false;
}
