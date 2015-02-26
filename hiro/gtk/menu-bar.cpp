namespace hiro {

auto pMenuBar::construct() -> void {
}

auto pMenuBar::destruct() -> void {
}

auto pMenuBar::append(shared_pointer<mMenu> menu) -> void {
  if(auto parent = _parent()) {
    parent->_append(*menu);
    if(menu->self()) {
      menu->self()->setFont(menu->font(true));
      menu->self()->setVisible(menu->visible(true));
    }
  }
}

auto pMenuBar::remove(shared_pointer<mMenu> menu) -> void {
}

auto pMenuBar::setEnabled(bool enabled) -> void {
  if(auto parent = _parent()) {
    parent->_setMenuEnabled(enabled);
  }
}

auto pMenuBar::setFont(const string& font) -> void {
  if(auto parent = _parent()) {
    parent->_setMenuFont(font);
  }
}

auto pMenuBar::setVisible(bool visible) -> void {
  if(auto parent = _parent()) {
    parent->_setMenuVisible(visible);
  }
}

auto pMenuBar::_parent() -> pWindow* {
  if(auto parent = self().parentWindow()) return parent->self();
  return nullptr;
}

}
