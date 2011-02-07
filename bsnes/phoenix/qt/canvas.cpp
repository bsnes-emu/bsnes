void Canvas::setParent(Layout &parent) {
  canvas->setParent(parent.widget->widget);
  canvas->show();
}

void Canvas::setGeometry(unsigned x, unsigned y, unsigned width, unsigned height) {
  delete canvas->image;
  canvas->image = new QImage(width, height, QImage::Format_RGB32);
  canvas->image->fill(0);
  canvas->setGeometry(x, y, width, height);
  canvas->update();
}

uint32_t* Canvas::buffer() {
  return (uint32_t*)canvas->image->bits();
}

void Canvas::redraw() {
  canvas->update();
}

Canvas::Canvas() {
  canvas = new Canvas::Data(*this);
  canvas->image = 0;
  widget->widget = canvas;
  canvas->image = new QImage(64, 64, QImage::Format_RGB32);
  canvas->image->fill(0);
}

Canvas::~Canvas() {
  if(canvas->image) delete canvas->image;
  delete canvas;
}

void Canvas::Data::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.drawImage(0, 0, *image);
}
