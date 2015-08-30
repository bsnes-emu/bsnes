#if defined(Hiro_StatusBar)

namespace hiro {

auto pStatusBar::construct() -> void {
  _setState();
}

auto pStatusBar::destruct() -> void {
}

auto pStatusBar::setEnabled(bool enabled) -> void {
  _setState();
}

auto pStatusBar::setFont(const Font& font) -> void {
  _setState();
}

auto pStatusBar::setText(const string& text) -> void {
  _setState();
}

auto pStatusBar::setVisible(bool visible) -> void {
  _setState();
}

auto pStatusBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pStatusBar::_setState() -> void {
  if(auto parent = _parent()) {
    parent->qtStatusBar->setFont(pFont::create(self().font(true)));
    parent->qtStatusBar->showMessage(QString::fromUtf8(state().text), 0);
    parent->qtStatusBar->setVisible(self().visible());
  }
}

}

#endif
