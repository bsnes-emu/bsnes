Size pDesktop::size() {
  QRect rect = QApplication::desktop()->screenGeometry();
  return { rect.width(), rect.height() };
}

Geometry pDesktop::workspace() {
  QRect rect = QApplication::desktop()->availableGeometry();
  return { rect.x(), rect.y(), rect.width(), rect.height() };
}
