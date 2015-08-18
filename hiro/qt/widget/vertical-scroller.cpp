#if defined(Hiro_VerticalScroller)

namespace hiro {

auto pVerticalScroller::construct() -> void {
  qtWidget = qtVerticalScroller = new QtVerticalScroller(*this);
  qtVerticalScroller->setRange(0, 100);
  qtVerticalScroller->setPageStep(101 >> 3);
  qtVerticalScroller->connect(qtVerticalScroller, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pVerticalScroller::destruct() -> void {
  delete qtVerticalScroller;
  qtWidget = qtVerticalScroller = nullptr;
}

auto pVerticalScroller::minimumSize() const -> Size {
  return {15, 0};
}

auto pVerticalScroller::setLength(unsigned length) -> void {
  _setState();
}

auto pVerticalScroller::setPosition(unsigned position) -> void {
  _setState();
}

auto pVerticalScroller::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtVerticalScroller->setRange(0, length - 1);
  qtVerticalScroller->setPageStep(length >> 3);
  qtVerticalScroller->setValue(state().position);
}

auto QtVerticalScroller::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
