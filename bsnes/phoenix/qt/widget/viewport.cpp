uintptr_t pViewport::handle() {
  return (uintptr_t)qtWidget->winId();
}

void pViewport::constructor() {
  qtWidget = new QWidget;
  qtWidget->setAttribute(Qt::WA_PaintOnScreen, true);
  qtWidget->setStyleSheet("background: #000000");
}

void pViewport::destructor() {
  delete qtWidget;
  qtWidget = 0;
}

void pViewport::orphan() {
  destructor();
  constructor();
}
