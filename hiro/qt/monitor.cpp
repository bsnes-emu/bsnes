#if defined(Hiro_Monitor)

namespace hiro {

auto pMonitor::count() -> uint {
  return QApplication::screens().count();
}

auto pMonitor::dpi(uint monitor) -> Position {
  return {
    QApplication::screens().at(monitor)->logicalDotsPerInchX(),
    QApplication::screens().at(monitor)->logicalDotsPerInchY()
  };
}

auto pMonitor::geometry(uint monitor) -> Geometry {
  QRect rectangle = QApplication::screens()[monitor]->geometry();
  return {rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()};
}

auto pMonitor::primary() -> uint {
  return max(QApplication::screens().indexOf(QApplication::primaryScreen()), 0);
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
