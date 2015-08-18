#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  QRect rect = QApplication::desktop()->screenGeometry();
  return {rect.width(), rect.height()};
}

auto pDesktop::workspace() -> Geometry {
  QRect rect = QApplication::desktop()->availableGeometry();
  return {rect.x(), rect.y(), rect.width(), rect.height()};
}

}

#endif
