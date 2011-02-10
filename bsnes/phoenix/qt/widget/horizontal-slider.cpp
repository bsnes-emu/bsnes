void HorizontalSlider::setLength(unsigned length) {
  length = length + (length == 0);
  horizontalSlider->setRange(0, length - 1);
  horizontalSlider->setPageStep(length >> 3);
}

unsigned HorizontalSlider::position() {
  return horizontalSlider->value();
}

void HorizontalSlider::setPosition(unsigned position) {
  horizontalSlider->setValue(position);
}

HorizontalSlider::HorizontalSlider() {
  horizontalSlider = new HorizontalSlider::Data(*this);
  widget->widget = horizontalSlider;
  horizontalSlider->connect(horizontalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
}
