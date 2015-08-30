#if defined(Hiro_MenuBar)

namespace hiro {

auto pMenuBar::construct() -> void {
  _setState();
}

auto pMenuBar::destruct() -> void {
}

auto pMenuBar::append(sMenu menu) -> void {
}

auto pMenuBar::remove(sMenu menu) -> void {
}

auto pMenuBar::setEnabled(bool enabled) -> void {
  _setState();
}

auto pMenuBar::setFont(const Font& font) -> void {
  _setState();
}

auto pMenuBar::setVisible(bool visible) -> void {
  _setState();
}

auto pMenuBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pMenuBar::_setState() -> void {
  if(auto parent = _parent()) {
    parent->qtMenuBar->setFont(pFont::create(self().font(true)));
    parent->qtMenuBar->setVisible(self().visible());
  }
}

}

#endif
