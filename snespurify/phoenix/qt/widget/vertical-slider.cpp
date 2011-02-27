unsigned pVerticalSlider::position() {
  return qtSlider->value();
}

void pVerticalSlider::setLength(unsigned length) {
  length += length == 0;
  qtSlider->setRange(0, length - 1);
  qtSlider->setPageStep(length >> 3);
}

void pVerticalSlider::setPosition(unsigned position) {
  qtSlider->setValue(position);
}

void pVerticalSlider::constructor() {
  qtWidget = qtSlider = new QSlider(Qt::Vertical);
  qtSlider->setRange(0, 100);
  qtSlider->setPageStep(101 >> 3);
  connect(qtSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
}

void pVerticalSlider::onChange() {
  verticalSlider.state.position = position();
  if(verticalSlider.onChange) verticalSlider.onChange();
}
