unsigned pHorizontalSlider::position() {
  return qtSlider->value();
}

void pHorizontalSlider::setLength(unsigned length) {
  length += length == 0;
  qtSlider->setRange(0, length - 1);
  qtSlider->setPageStep(length >> 3);
}

void pHorizontalSlider::setPosition(unsigned position) {
  qtSlider->setValue(position);
}

pHorizontalSlider::pHorizontalSlider(HorizontalSlider &horizontalSlider) : horizontalSlider(horizontalSlider), pWidget(horizontalSlider) {
  qtWidget = qtSlider = new QSlider(Qt::Horizontal);
  qtSlider->setRange(0, 100);
  qtSlider->setPageStep(101 >> 3);
  connect(qtSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
}

void pHorizontalSlider::onChange() {
  horizontalSlider.state.position = position();
  if(horizontalSlider.onChange) horizontalSlider.onChange();
}
