#if defined(Hiro_CheckLabel)

namespace hiro {

auto pCheckLabel::construct() -> void {
  qtWidget = qtCheckLabel = new QtCheckLabel(*this);
  qtCheckLabel->connect(qtCheckLabel, SIGNAL(stateChanged(int)), SLOT(onToggle()));

  pWidget::construct();
  _setState();
}

auto pCheckLabel::destruct() -> void {
  delete qtCheckLabel;
  qtWidget = qtCheckLabel = nullptr;
}

auto pCheckLabel::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text);
  return {size.width() + 26, size.height() + 6};
}

auto pCheckLabel::setChecked(bool checked) -> void {
  _setState();
}

auto pCheckLabel::setText(const string& text) -> void {
  _setState();
}

auto pCheckLabel::_setState() -> void {
  lock();
  qtCheckLabel->setChecked(state().checked);
  qtCheckLabel->setText(QString::fromUtf8(state().text));
  unlock();
}

auto QtCheckLabel::onToggle() -> void {
  p.state().checked = p.qtCheckLabel->isChecked();
  if(!p.locked()) p.self().doToggle();
}

}

#endif
