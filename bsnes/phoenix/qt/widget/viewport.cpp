uintptr_t pViewport::handle() {
  return (uintptr_t)qtWidget->winId();
}

pViewport::pViewport(Viewport &viewport) : viewport(viewport), pWidget(viewport) {
  qtWidget = new QWidget;
  qtWidget->setAttribute(Qt::WA_PaintOnScreen, true);
  qtWidget->setStyleSheet("background: #000000");
}
