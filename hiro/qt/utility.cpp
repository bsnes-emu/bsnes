namespace hiro {

static auto CalculateAlignment(Alignment alignment) -> signed {
  signed result = 0;
  if(alignment.horizontal() < 0.333) result |= Qt::AlignLeft;
  else if(alignment.horizontal() > 0.666) result |= Qt::AlignRight;
  else result |= Qt::AlignCenter;
  if(alignment.vertical() < 0.333) result |= Qt::AlignTop;
  else if(alignment.vertical() > 0.666) result |= Qt::AlignBottom;
  else result |= Qt::AlignVCenter;
  return result;
}

static auto CreateBrush(Color color) -> QBrush {
  return color ? QColor(color.red(), color.green(), color.blue()) : QBrush();
}

static auto CreateColor(Color color, QColor fallback = {}) -> QColor {
  return color ? QColor(color.red(), color.green(), color.blue()) : fallback;
}

static auto CreateIcon(const image& icon, bool scale = false) -> QIcon {
  if(!icon) return QIcon();
  auto qtBuffer = icon;
  qtBuffer.transform();
  if(scale) qtBuffer.scale(16, 16, true);
  QImage qtImage(qtBuffer.data(), qtBuffer.width(), qtBuffer.height(), QImage::Format_ARGB32);
  return QIcon(QPixmap::fromImage(qtImage));
}

static auto DropPaths(QDropEvent* event) -> vector<string> {
  QList<QUrl> urls = event->mimeData()->urls();
  if(urls.size() == 0) return {};

  vector<string> paths;
  for(auto n : range(urls.size())) {
    string path{urls[n].path().toUtf8().constData()};
    if(!path) continue;
    if(directory::exists(path) && !path.endsWith("/")) path.append("/");
    paths.append(path);
  }

  return paths;
}

}
