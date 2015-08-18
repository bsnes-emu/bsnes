#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> unsigned {
  return QApplication::desktop()->screenCount();
}

auto pMonitor::geometry(unsigned monitor) -> Geometry {
  QRect rectangle = QApplication::desktop()->screenGeometry(monitor);
  return {rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};
}

auto pMonitor::primary() -> unsigned {
  return QApplication::desktop()->primaryScreen();
}

}

#endif
