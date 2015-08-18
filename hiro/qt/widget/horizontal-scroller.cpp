#if defined(Hiro_HorizontalScroller)

namespace hiro {

auto pHorizontalScroller::construct() -> void {
  qtWidget = qtHorizontalScroller = new QtHorizontalScroller(*this);
  qtHorizontalScroller->setRange(0, 100);
  qtHorizontalScroller->setPageStep(101 >> 3);
  qtHorizontalScroller->connect(qtHorizontalScroller, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pHorizontalScroller::destruct() -> void {
  delete qtHorizontalScroller;
  qtWidget = qtHorizontalScroller = nullptr;
}

auto pHorizontalScroller::minimumSize() const -> Size {
  return {0, 15};
}

auto pHorizontalScroller::setLength(unsigned length) -> void {
  _setState();
}

auto pHorizontalScroller::setPosition(unsigned position) -> void {
  _setState();
}

auto pHorizontalScroller::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtHorizontalScroller->setRange(0, length - 1);
  qtHorizontalScroller->setPageStep(length >> 3);
  qtHorizontalScroller->setValue(state().position);
}

auto QtHorizontalScroller::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
