uintptr_t Viewport::handle() {
  return (uintptr_t)viewport->winId();
}

Viewport::Viewport() {
  viewport = new Viewport::Data(*this);
  widget->widget = viewport;
  viewport->setAttribute(Qt::WA_PaintOnScreen, true);
  viewport->setStyleSheet("background: #000000");
}
