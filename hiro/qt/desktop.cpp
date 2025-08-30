#if defined(Hiro_Desktop)

namespace hiro {

auto pDesktop::size() -> Size {
  QRect rect;
  for(QScreen* screen : QApplication::screens()) {
    rect = rect.united(screen->geometry());
  }
  return {rect.width(), rect.height()};
}

auto pDesktop::workspace() -> Geometry {
  QRect rect;
  for(QScreen* screen : QApplication::screens()) {
    rect = rect.united(screen->geometry());
  }
  return {rect.x(), rect.y(), rect.width(), rect.height()};
}

}

#endif
