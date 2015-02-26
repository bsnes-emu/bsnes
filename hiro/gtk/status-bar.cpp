namespace hiro {

auto pStatusBar::construct() -> void {
}

auto pStatusBar::destruct() -> void {
}

auto pStatusBar::setEnabled(bool enabled) -> void {
  if(auto parent = _parent()) {
    parent->_setStatusEnabled(enabled);
  }
}

auto pStatusBar::setFont(const string& font) -> void {
  if(auto parent = _parent()) {
    parent->_setStatusFont(font);
  }
}

auto pStatusBar::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    parent->_setStatusText(text);
  }
}

auto pStatusBar::setVisible(bool visible) -> void {
  if(auto parent = _parent()) {
    parent->_setStatusVisible(visible);
  }
}

auto pStatusBar::_parent() -> pWindow* {
  if(auto parent = self().parentWindow()) return parent->self();
  return nullptr;
}

}
