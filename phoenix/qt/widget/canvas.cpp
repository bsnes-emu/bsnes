namespace phoenix {

void pCanvas::setDroppable(bool droppable) {
  qtCanvas->setAcceptDrops(droppable);
}

void pCanvas::setGeometry(Geometry geometry) {
  if(canvas.state.width == 0 || canvas.state.height == 0) rasterize();
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width < geometry.width) {
    geometry.x += (geometry.width - width) / 2;
    geometry.width = width;
  }

  if(height < geometry.height) {
    geometry.y += (geometry.height - height) / 2;
    geometry.height = height;
  }

  pWidget::setGeometry(geometry);
}

void pCanvas::setMode(Canvas::Mode mode) {
  rasterize();
  qtCanvas->update();
}

void pCanvas::setSize(Size size) {
  rasterize();
  qtCanvas->update();
}

void pCanvas::constructor() {
  qtWidget = qtCanvas = new QtCanvas(*this);
  qtCanvas->setMouseTracking(true);

  pWidget::synchronizeState();
  rasterize(), qtCanvas->update();
}

void pCanvas::destructor() {
  release();
  delete qtCanvas;
  qtWidget = qtCanvas = nullptr;
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::rasterize() {
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(canvas.state.mode != Canvas::Mode::Color) {
    if(width != surfaceWidth || height != surfaceHeight) release();
    if(!surface) surface = new QImage(width, height, QImage::Format_ARGB32);
  }

  if(canvas.state.mode == Canvas::Mode::Gradient) {
    nall::image image;
    image.allocate(width, height);
    image.linearGradient(
      canvas.state.gradient[0].argb(), canvas.state.gradient[1].argb(), canvas.state.gradient[2].argb(), canvas.state.gradient[3].argb()
    );
    memcpy(surface->bits(), image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Image) {
    nall::image image = canvas.state.image;
    image.scale(width, height);
    image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    memcpy(surface->bits(), image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Data) {
    if(width == canvas.state.width && height == canvas.state.height) {
      memcpy(surface->bits(), canvas.state.data, width * height * sizeof(uint32_t));
    } else {
      memset(surface->bits(), 0x00, width * height * sizeof(uint32_t));
    }
  }

  surfaceWidth = width;
  surfaceHeight = height;
}

void pCanvas::release() {
  if(surface == nullptr) return;
  delete surface;
  surface = nullptr;
  surfaceWidth = 0;
  surfaceHeight = 0;
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
  Canvas& canvas = self.canvas;
  QPainter painter(self.qtCanvas);

  if(canvas.state.mode == Canvas::Mode::Color) {
    painter.fillRect(event->rect(), QBrush(QColor(canvas.state.color.red, canvas.state.color.green, canvas.state.color.blue, canvas.state.color.alpha)));
  } else {
    painter.drawImage(0, 0, *self.surface);
  }
}

pCanvas::QtCanvas::QtCanvas(pCanvas& self) : self(self) {
}

}
