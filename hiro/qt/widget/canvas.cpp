#if defined(Hiro_Canvas)

namespace hiro {

auto pCanvas::construct() -> void {
  qtWidget = qtCanvas = new QtCanvas(*this);
  qtCanvas->setMouseTracking(true);

  pWidget::construct();
  _rasterize();
  qtCanvas->update();
}

auto pCanvas::destruct() -> void {
  _release();
  delete qtCanvas;
  qtWidget = qtCanvas = nullptr;
}

auto pCanvas::minimumSize() const -> Size {
  if(auto& icon = state().icon) return {icon.width(), icon.height()};
  return {0, 0};
}

auto pCanvas::setColor(Color color) -> void {
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  qtCanvas->setAcceptDrops(droppable);
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  update();
  pWidget::setGeometry(geometry);
}

auto pCanvas::setGradient(Gradient gradient) -> void {
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  qtCanvas->update();
}

auto pCanvas::_rasterize() -> void {
  int width = 0;
  int height = 0;

  if(auto& icon = state().icon) {
    width = icon.width();
    height = icon.height();
  } else {
    width = pSizable::state().geometry.width();
    height = pSizable::state().geometry.height();
  }
  if(width <= 0 || height <= 0) return;

  if(width != qtImageWidth || height != qtImageHeight) _release();
  qtImageWidth = width;
  qtImageHeight = height;

  if(!qtImage) qtImage = new QImage(width, height, QImage::Format_ARGB32);
  auto buffer = (uint32_t*)qtImage->bits();

  if(auto& icon = state().icon) {
    memory::copy<uint32_t>(buffer, state().icon.data(), width * height);
  } else if(auto& gradient = state().gradient) {
    auto& colors = gradient.state.colors;
    image fill;
    fill.allocate(width, height);
    fill.gradient(colors[0].value(), colors[1].value(), colors[2].value(), colors[3].value());
    memory::copy(buffer, fill.data(), fill.size());
  } else {
    uint32_t color = state().color.value();
    for(auto n : range(width * height)) buffer[n] = color;
  }
}

auto pCanvas::_release() -> void {
  if(qtImage) {
    delete qtImage;
    qtImage = nullptr;
  }
  qtImageWidth = 0;
  qtImageHeight = 0;
}

auto QtCanvas::dragEnterEvent(QDragEnterEvent* event) -> void {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

auto QtCanvas::dropEvent(QDropEvent* event) -> void {
  if(auto paths = DropPaths(event)) p.self().doDrop(paths);
}

auto QtCanvas::leaveEvent(QEvent* event) -> void {
  p.self().doMouseLeave();
}

auto QtCanvas::mouseMoveEvent(QMouseEvent* event) -> void {
  p.self().doMouseMove({event->pos().x(), event->pos().y()});
}

auto QtCanvas::mousePressEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMousePress(Mouse::Button::Left); break;
  case Qt::MidButton: p.self().doMousePress(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMousePress(Mouse::Button::Right); break;
  }
}

auto QtCanvas::mouseReleaseEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMouseRelease(Mouse::Button::Left); break;
  case Qt::MidButton: p.self().doMouseRelease(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMouseRelease(Mouse::Button::Right); break;
  }
}

auto QtCanvas::paintEvent(QPaintEvent* event) -> void {
  if(!p.qtImage) return;

  signed sx = 0, sy = 0, dx = 0, dy = 0;
  signed width = p.qtImageWidth;
  signed height = p.qtImageHeight;
  auto geometry = p.pSizable::state().geometry;

  if(width <= geometry.width()) {
    sx = 0;
    dx = (geometry.width() - width) / 2;
  } else {
    sx = (width - geometry.width()) / 2;
    dx = 0;
    width = geometry.width();
  }

  if(height <= geometry.height()) {
    sy = 0;
    dy = (geometry.height() - height) / 2;
  } else {
    sy = (height - geometry.height()) / 2;
    dy = 0;
    height = geometry.height();
  }

  QPainter painter(p.qtCanvas);
  painter.drawImage(dx, dy, *p.qtImage, sx, sy, width, height);
}

}

#endif
