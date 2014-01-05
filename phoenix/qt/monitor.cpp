namespace phoenix {

unsigned pMonitor::count() {
  return QApplication::desktop()->screenCount();
}

Geometry pMonitor::geometry(unsigned monitor) {
  QRect rectangle = QApplication::desktop()->screenGeometry(monitor);
  return {rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};
}

unsigned pMonitor::primary() {
  return QApplication::desktop()->primaryScreen();
}

}
