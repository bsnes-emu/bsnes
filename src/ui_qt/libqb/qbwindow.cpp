void QbWindow::showHidden(bool state) {
  #if !defined(PLATFORM_X)
  if(state) {
    setWindowOpacity(0.0);
  } else {
    setWindowOpacity(1.0);
  }
  showNormal();
  #endif
}

void QbWindow::showAt(double x, double y) {
  #if defined(PLATFORM_X)
  resize(0, 0);
  showNormal();
  #else
  if(isVisible() == false) showHidden(true);
  resize(0, 0);
  QApplication::processEvents();

  QRect deskRect = QApplication::desktop()->availableGeometry(this);

  //convert cartesian coordinates to linear scale (-1.0 -> 0.0, 0.0 -> 0.5, +1.0 -> +1.0)
  x = (x / 2.0) + 0.5;
  y = (y / 2.0) + 0.5;

  unsigned availableWidth  = deskRect.width()  - frameSize().width();
  unsigned availableHeight = deskRect.height() - frameSize().height();

  move(deskRect.left() + availableWidth * x, deskRect.top() + availableHeight * y);
  showHidden(false);
  #endif
}

void QbWindow::setFocus() {
  QApplication::processEvents();
  activateWindow();
  raise();
}
