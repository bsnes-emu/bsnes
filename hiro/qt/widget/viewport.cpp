#if defined(Hiro_Viewport)

namespace hiro {

auto pViewport::construct() -> void {
  qtWidget = qtViewport = new QtViewport(*this);
  qtViewport->setMouseTracking(true);
  qtViewport->setAttribute(Qt::WA_PaintOnScreen, true);
  qtViewport->setStyleSheet("background: #000000");

  pWidget::construct();
  _setState();
}

auto pViewport::destruct() -> void {
  delete qtViewport;
  qtWidget = qtViewport = nullptr;
}

auto pViewport::handle() const -> uintptr_t {
  return (uintptr_t)qtViewport->winId();
}

auto pViewport::setDroppable(bool droppable) -> void {
  _setState();
}

auto pViewport::setFocusable(bool focusable) -> void {
  //TODO
}

auto pViewport::_setState() -> void {
  qtViewport->setAcceptDrops(self().droppable());
}

auto QtViewport::dragEnterEvent(QDragEnterEvent* event) -> void {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

auto QtViewport::dropEvent(QDropEvent* event) -> void {
  if(auto paths = DropPaths(event)) p.self().doDrop(paths);
}

auto QtViewport::leaveEvent(QEvent* event) -> void {
  p.self().doMouseLeave();
}

auto QtViewport::mouseMoveEvent(QMouseEvent* event) -> void {
  p.self().doMouseMove({event->pos().x(), event->pos().y()});
}

auto QtViewport::mousePressEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMousePress(Mouse::Button::Left); break;
  case Qt::MiddleButton: p.self().doMousePress(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMousePress(Mouse::Button::Right); break;
  }
}

auto QtViewport::mouseReleaseEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMouseRelease(Mouse::Button::Left); break;
  case Qt::MiddleButton: p.self().doMouseRelease(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMouseRelease(Mouse::Button::Right); break;
  }
}

}

#endif
