Geometry pVerticalScrollBar::minimumGeometry() {
  return { 0, 0, 15, 0 };
}

unsigned pVerticalScrollBar::position() {
  return qtScrollBar->value();
}

void pVerticalScrollBar::setLength(unsigned length) {
  length += length == 0;
  qtScrollBar->setRange(0, length - 1);
  qtScrollBar->setPageStep(length >> 3);
}

void pVerticalScrollBar::setPosition(unsigned position) {
  qtScrollBar->setValue(position);
}

void pVerticalScrollBar::constructor() {
  qtWidget = qtScrollBar = new QScrollBar(Qt::Vertical);
  qtScrollBar->setRange(0, 100);
  qtScrollBar->setPageStep(101 >> 3);
  connect(qtScrollBar, SIGNAL(valueChanged(int)), SLOT(onChange()));

  setLength(verticalScrollBar.state.length);
  setPosition(verticalScrollBar.state.position);
}

void pVerticalScrollBar::destructor() {
  delete qtScrollBar;
  qtWidget = qtScrollBar = 0;
}

void pVerticalScrollBar::orphan() {
  destructor();
  constructor();
}

void pVerticalScrollBar::onChange() {
  verticalScrollBar.state.position = position();
  if(verticalScrollBar.onChange) verticalScrollBar.onChange();
}
