namespace phoenix {

Size pHorizontalScroller::minimumSize() {
  return {0, 15};
}

unsigned pHorizontalScroller::position() {
  return qtScroller->value();
}

void pHorizontalScroller::setLength(unsigned length) {
  length += length == 0;
  qtScroller->setRange(0, length - 1);
  qtScroller->setPageStep(length >> 3);
}

void pHorizontalScroller::setPosition(unsigned position) {
  qtScroller->setValue(position);
}

void pHorizontalScroller::constructor() {
  qtWidget = qtScroller = new QScrollBar(Qt::Horizontal);
  qtScroller->setRange(0, 100);
  qtScroller->setPageStep(101 >> 3);
  connect(qtScroller, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::synchronizeState();
  setLength(horizontalScroller.state.length);
  setPosition(horizontalScroller.state.position);
}

void pHorizontalScroller::destructor() {
  delete qtScroller;
  qtWidget = qtScroller = nullptr;
}

void pHorizontalScroller::orphan() {
  destructor();
  constructor();
}

void pHorizontalScroller::onChange() {
  horizontalScroller.state.position = position();
  if(horizontalScroller.onChange) horizontalScroller.onChange();
}

}
