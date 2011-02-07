void VerticalSlider::setLength(unsigned length) {
  length = length + (length == 0);
  verticalSlider->setRange(0, length - 1);
  verticalSlider->setPageStep(length >> 3);
}

unsigned VerticalSlider::position() {
  return verticalSlider->value();
}

void VerticalSlider::setPosition(unsigned position) {
  verticalSlider->setValue(position);
}

VerticalSlider::VerticalSlider() {
  verticalSlider = new VerticalSlider::Data(*this);
  widget->widget = verticalSlider;
  verticalSlider->setInvertedAppearance(true);
  verticalSlider->setInvertedControls(true);
  verticalSlider->connect(verticalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
}
