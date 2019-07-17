#if defined(Hiro_ProgressBar)

namespace hiro {

auto pProgressBar::construct() -> void {
  qtWidget = qtProgressBar = new QProgressBar;
  qtProgressBar->setRange(0, 100);
  qtProgressBar->setTextVisible(false);

  pWidget::construct();
  _setState();
}

auto pProgressBar::destruct() -> void {
  delete qtProgressBar;
  qtWidget = qtProgressBar = nullptr;
}

auto pProgressBar::minimumSize() const -> Size {
  return {0, 25};
}

auto pProgressBar::setPosition(unsigned position) -> void {
  _setState();
}

auto pProgressBar::_setState() -> void {
  qtProgressBar->setValue(state().position);
}

}

#endif
