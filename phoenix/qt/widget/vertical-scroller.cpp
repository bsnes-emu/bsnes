namespace phoenix {

Size pVerticalScroller::minimumSize() {
  return {15, 0};
}

void pVerticalScroller::setLength(unsigned length) {
  length += length == 0;
  qtScroller->setRange(0, length - 1);
  qtScroller->setPageStep(length >> 3);
}

void pVerticalScroller::setPosition(unsigned position) {
  qtScroller->setValue(position);
}

void pVerticalScroller::constructor() {
  qtWidget = qtScroller = new QScrollBar(Qt::Vertical);
  qtScroller->setRange(0, 100);
  qtScroller->setPageStep(101 >> 3);
  connect(qtScroller, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::synchronizeState();
  setLength(verticalScroller.state.length);
  setPosition(verticalScroller.state.position);
}

void pVerticalScroller::destructor() {
  delete qtScroller;
  qtWidget = qtScroller = nullptr;
}

void pVerticalScroller::orphan() {
  destructor();
  constructor();
}

void pVerticalScroller::onChange() {
  verticalScroller.state.position = qtScroller->value();
  if(verticalScroller.onChange) verticalScroller.onChange();
}

}
