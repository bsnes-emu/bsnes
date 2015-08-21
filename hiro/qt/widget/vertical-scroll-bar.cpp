#if defined(Hiro_VerticalScrollBar)

namespace hiro {

auto pVerticalScrollBar::construct() -> void {
  qtWidget = qtVerticalScrollBar = new QtVerticalScrollBar(*this);
  qtVerticalScrollBar->setRange(0, 100);
  qtVerticalScrollBar->setPageStep(101 >> 3);
  qtVerticalScrollBar->connect(qtVerticalScrollBar, SIGNAL(valueChanged(int)), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pVerticalScrollBar::destruct() -> void {
  delete qtVerticalScrollBar;
  qtWidget = qtVerticalScrollBar = nullptr;
}

auto pVerticalScrollBar::minimumSize() const -> Size {
  return {15, 0};
}

auto pVerticalScrollBar::setLength(unsigned length) -> void {
  _setState();
}

auto pVerticalScrollBar::setPosition(unsigned position) -> void {
  _setState();
}

auto pVerticalScrollBar::_setState() -> void {
  signed length = state().length + (state().length == 0);
  qtVerticalScrollBar->setRange(0, length - 1);
  qtVerticalScrollBar->setPageStep(length >> 3);
  qtVerticalScrollBar->setValue(state().position);
}

auto QtVerticalScrollBar::onChange() -> void {
  p.state().position = value();
  p.self().doChange();
}

}

#endif
