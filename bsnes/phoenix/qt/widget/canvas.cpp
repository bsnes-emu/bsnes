uint32_t* pCanvas::buffer() {
  return (uint32_t*)qtImage->bits();
}

void pCanvas::setGeometry(const Geometry &geometry) {
  qtImage = new QImage(geometry.width, geometry.height, QImage::Format_RGB32);
  qtImage->fill(0);
  update();
  pWidget::setGeometry(geometry);
}

void pCanvas::update() {
  qtCanvas->update();
}

void pCanvas::constructor() {
  qtWidget = qtCanvas = new QtCanvas(*this);
  qtImage = new QImage(256, 256, QImage::Format_RGB32);

  pWidget::synchronizeState();
  update();
}

void pCanvas::destructor() {
  delete qtCanvas;
  delete qtImage;
  qtWidget = qtCanvas = 0;
  qtImage = 0;
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::QtCanvas::paintEvent(QPaintEvent *event) {
  QPainter painter(self.qtCanvas);
  painter.drawImage(0, 0, *self.qtImage);
}

pCanvas::QtCanvas::QtCanvas(pCanvas &self) : self(self) {
}
