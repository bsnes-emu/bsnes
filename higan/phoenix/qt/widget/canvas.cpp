namespace phoenix {

void pCanvas::setDroppable(bool droppable) {
  qtCanvas->setAcceptDrops(droppable);
}

void pCanvas::setSize(Size size) {
  delete qtImage;
  qtImage = new QImage(size.width, size.height, QImage::Format_ARGB32);
}

void pCanvas::update() {
  uint32_t* dp = (uint32_t*)qtImage->bits(), *sp = (uint32_t*)canvas.state.data;
  for(unsigned n = 0; n < canvas.state.width * canvas.state.height; n++) *dp++ = 0xff000000 | *sp++;
  qtCanvas->update();
}

void pCanvas::constructor() {
  qtWidget = qtCanvas = new QtCanvas(*this);
  qtCanvas->setMouseTracking(true);
  qtImage = new QImage(canvas.state.width, canvas.state.height, QImage::Format_ARGB32);
  memcpy(qtImage->bits(), canvas.state.data, canvas.state.width * canvas.state.height * sizeof(uint32_t));

  pWidget::synchronizeState();
  update();
}

void pCanvas::destructor() {
  delete qtCanvas;
  delete qtImage;
  qtWidget = qtCanvas = nullptr;
  qtImage = nullptr;
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::QtCanvas::dragEnterEvent(QDragEnterEvent* event) {
  if(event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void pCanvas::QtCanvas::dropEvent(QDropEvent* event) {
  lstring paths = DropPaths(event);
  if(paths.empty()) return;
  if(self.canvas.onDrop) self.canvas.onDrop(paths);
}

void pCanvas::QtCanvas::leaveEvent(QEvent* event) {
  if(self.canvas.onMouseLeave) self.canvas.onMouseLeave();
}

void pCanvas::QtCanvas::mouseMoveEvent(QMouseEvent* event) {
  if(self.canvas.onMouseMove) self.canvas.onMouseMove({event->pos().x(), event->pos().y()});
}

void pCanvas::QtCanvas::mousePressEvent(QMouseEvent* event) {
  if(!self.canvas.onMousePress) return;
  switch(event->button()) {
  case Qt::LeftButton: self.canvas.onMousePress(Mouse::Button::Left); break;
  case Qt::MidButton: self.canvas.onMousePress(Mouse::Button::Middle); break;
  case Qt::RightButton: self.canvas.onMousePress(Mouse::Button::Right); break;
  }
}

void pCanvas::QtCanvas::mouseReleaseEvent(QMouseEvent* event) {
  if(!self.canvas.onMouseRelease) return;
  switch(event->button()) {
  case Qt::LeftButton: self.canvas.onMouseRelease(Mouse::Button::Left); break;
  case Qt::MidButton: self.canvas.onMouseRelease(Mouse::Button::Middle); break;
  case Qt::RightButton: self.canvas.onMouseRelease(Mouse::Button::Right); break;
  }
}

void pCanvas::QtCanvas::paintEvent(QPaintEvent* event) {
  QPainter painter(self.qtCanvas);
  painter.drawImage(0, 0, *self.qtImage);

//this will scale the source image to fit the target widget size (nearest-neighbor):
//painter.drawImage(
//  QRect(0, 0, geometry().width(), geometry().height()),
//  *self.qtImage,
//  QRect(0, 0, self.canvas.state.width, self.canvas.state.height)
//);
}

pCanvas::QtCanvas::QtCanvas(pCanvas& self) : self(self) {
}

}
