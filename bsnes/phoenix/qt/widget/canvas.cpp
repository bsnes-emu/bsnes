void pCanvas::setSize(const Size &size) {
  delete qtImage;
  qtImage = new QImage(size.width, size.height, QImage::Format_ARGB32);
}

void pCanvas::update() {
  uint32_t *dp = (uint32_t*)qtImage->bits(), *sp = (uint32_t*)canvas.state.data;
  for(unsigned n = 0; n < canvas.state.width * canvas.state.height; n++) *dp++ = 0xff000000 | *sp++;
  qtCanvas->update();
}

void pCanvas::constructor() {
  qtWidget = qtCanvas = new QtCanvas(*this);
  qtImage = new QImage(canvas.state.width, canvas.state.height, QImage::Format_ARGB32);
  memcpy(qtImage->bits(), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));

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

//this will scale the source image to fit the target widget size (nearest-neighbor):
//painter.drawImage(
//  QRect(0, 0, geometry().width(), geometry().height()),
//  *self.qtImage,
//  QRect(0, 0, self.canvas.state.width, self.canvas.state.height)
//);
}

pCanvas::QtCanvas::QtCanvas(pCanvas &self) : self(self) {
}
