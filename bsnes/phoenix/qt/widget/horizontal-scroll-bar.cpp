Geometry pHorizontalScrollBar::minimumGeometry() {
  return { 0, 0, 0, 15 };
}

unsigned pHorizontalScrollBar::position() {
  return qtScrollBar->value();
}

void pHorizontalScrollBar::setLength(unsigned length) {
  length += length == 0;
  qtScrollBar->setRange(0, length - 1);
  qtScrollBar->setPageStep(length >> 3);
}

void pHorizontalScrollBar::setPosition(unsigned position) {
  qtScrollBar->setValue(position);
}

void pHorizontalScrollBar::constructor() {
  qtWidget = qtScrollBar = new QScrollBar(Qt::Horizontal);
  qtScrollBar->setRange(0, 100);
  qtScrollBar->setPageStep(101 >> 3);
  connect(qtScrollBar, SIGNAL(valueChanged(int)), SLOT(onChange()));
}

void pHorizontalScrollBar::onChange() {
  horizontalScrollBar.state.position = position();
  if(horizontalScrollBar.onChange) horizontalScrollBar.onChange();
}
