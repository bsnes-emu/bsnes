void HorizontalSlider::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length) {
  length += (length == 0);
  horizontalSlider->setParent(parent.window->container);
  horizontalSlider->setGeometry(x, y, width, height);
  horizontalSlider->setRange(0, length - 1);
  horizontalSlider->setPageStep(length >> 3);
  horizontalSlider->connect(horizontalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));
  horizontalSlider->show();
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
}
