uintptr_t pViewport::handle() {
  return (uintptr_t)qtWidget->winId();
}

void pViewport::constructor() {
  qtWidget = new QWidget;
  qtWidget->setAttribute(Qt::WA_PaintOnScreen, true);
  qtWidget->setStyleSheet("background: #000000");
}
