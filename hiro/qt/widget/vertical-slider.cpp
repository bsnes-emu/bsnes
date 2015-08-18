#if defined(Hiro_VerticalSlider)

namespace hiro {

auto pVerticalSlider::minimumSize() const -> Size {
  return {20, 0};
}

auto pVerticalSlider::setLength(unsigned length) -> void {
  _setState();
}

auto pVerticalSlider::setPosition(unsigned position) -> void {
  _setState();
}

auto pVerticalSlider::construct() -> void {
  qtWidget = qtVerticalSlider = new QtVerticalSlider(*this);
  qtVerticalSlider->setInvertedAppearance(true);
  qtVerticalSlider->setRange(0, 100);
  qtVerticalSlider->setPageStep(101 >> 3);
  qtVerticalSlider->connect(qtVerticalSlider, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pVerticalSlider::destruct() -> void {
  delete qtVerticalSlider;
  qtWidget = qtVerticalSlider = nullptr;
}

auto pVerticalSlider::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtVerticalSlider->setRange(0, length - 1);
  qtVerticalSlider->setPageStep(length >> 3);
  qtVerticalSlider->setValue(state().position);
}

auto QtVerticalSlider::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
