namespace phoenix {

Position pMouse::position() {
  QPoint point = QCursor::pos();
  return {point.x(), point.y()};
}

bool pMouse::pressed(Mouse::Button button) {
  Qt::MouseButtons buttons = QApplication::mouseButtons();
  switch(button) {
  case Mouse::Button::Left: return buttons & Qt::LeftButton;
  case Mouse::Button::Middle: return buttons & Qt::MidButton;
  case Mouse::Button::Right: return buttons & Qt::RightButton;
  }
  return false;
}

}
