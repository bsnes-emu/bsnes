#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  return QApplication::desktop()->screenCount();
}

auto pMonitor::dpi(uint monitor) -> Position {
  //Qt does not support per-monitor DPI retrieval
  return {
    QApplication::desktop()->logicalDpiX(),
    QApplication::desktop()->logicalDpiY()
  };
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  QRect rectangle = QApplication::desktop()->screenGeometry(monitor);
  return {rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};
}

auto pMonitor::primary() -> uint {
  return QApplication::desktop()->primaryScreen();
}

auto pMonitor::workspace(uint monitor) -> Geometry {
  if(Monitor::count() == 1) {
    return Desktop::workspace();
  } else {
    return Monitor::geometry(monitor);
  }
}

}

#endif
