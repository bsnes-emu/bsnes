#if defined(Hiro_Mouse)

namespace hiro {

auto pMouse::position() -> Position {
  QPoint point = QCursor::pos();
  return {point.x(), point.y()};
}

auto pMouse::pressed(Mouse::Button button) -> bool {
  Qt::MouseButtons buttons = QApplication::mouseButtons();
  switch(button) {
  case Mouse::Button::Left: return buttons & Qt::LeftButton;
  case Mouse::Button::Middle: return buttons & Qt::MiddleButton;
  case Mouse::Button::Right: return buttons & Qt::RightButton;
  }
  return false;
}

}

#endif
