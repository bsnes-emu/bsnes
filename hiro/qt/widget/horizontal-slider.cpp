#if defined(Hiro_HorizontalSlider)

namespace hiro {

auto pHorizontalSlider::minimumSize() const -> Size {
  return {0, 20};
}

auto pHorizontalSlider::setLength(unsigned length) -> void {
  _setState();
}

auto pHorizontalSlider::setPosition(unsigned position) -> void {
  _setState();
}

auto pHorizontalSlider::construct() -> void {
  qtWidget = qtHorizontalSlider = new QtHorizontalSlider(*this);
  qtHorizontalSlider->setRange(0, 100);
  qtHorizontalSlider->setPageStep(101 >> 3);
  qtHorizontalSlider->connect(qtHorizontalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pHorizontalSlider::destruct() -> void {
  delete qtHorizontalSlider;
  qtWidget = qtHorizontalSlider = nullptr;
}

auto pHorizontalSlider::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtHorizontalSlider->setRange(0, length - 1);
  qtHorizontalSlider->setPageStep(length >> 3);
  qtHorizontalSlider->setValue(state().position);
}

auto QtHorizontalSlider::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
