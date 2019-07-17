#if defined(Hiro_HorizontalScrollBar)

namespace hiro {

auto pHorizontalScrollBar::construct() -> void {
  qtWidget = qtHorizontalScrollBar = new QtHorizontalScrollBar(*this);
  qtHorizontalScrollBar->setRange(0, 100);
  qtHorizontalScrollBar->setPageStep(101 >> 3);
  qtHorizontalScrollBar->connect(qtHorizontalScrollBar, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pHorizontalScrollBar::destruct() -> void {
  delete qtHorizontalScrollBar;
  qtWidget = qtHorizontalScrollBar = nullptr;
}

auto pHorizontalScrollBar::minimumSize() const -> Size {
  return {0, 15};
}

auto pHorizontalScrollBar::setLength(unsigned length) -> void {
  _setState();
}

auto pHorizontalScrollBar::setPosition(unsigned position) -> void {
  _setState();
}

auto pHorizontalScrollBar::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtHorizontalScrollBar->setRange(0, length - 1);
  qtHorizontalScrollBar->setPageStep(length >> 3);
  qtHorizontalScrollBar->setValue(state().position);
}

auto QtHorizontalScrollBar::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
