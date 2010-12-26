void VerticalSlider::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length) {
  length += (length == 0);
  verticalSlider->setParent(parent.window->container);
  verticalSlider->setGeometry(x, y, width, height);
  verticalSlider->setInvertedAppearance(true);
  verticalSlider->setInvertedControls(true);
  verticalSlider->setRange(0, length - 1);
  verticalSlider->setPageStep(length >> 3);
  verticalSlider->connect(verticalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
  verticalSlider->show();
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
}
