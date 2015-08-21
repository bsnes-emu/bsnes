#if defined(Hiro_Action)

namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
  _setState();
}

auto pAction::setFont(const string& font) -> void {
  _setState();
}

auto pAction::setVisible(bool visible) -> void {
  _setState();
}

auto pAction::_parentMenu() -> maybe<pMenu&> {
  if(auto parent = self().parentMenu()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_parentMenuBar() -> maybe<pMenuBar&> {
  if(auto parent = self().parentMenuBar()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_parentPopupMenu() -> maybe<pPopupMenu&> {
  if(auto parent = self().parentPopupMenu()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_setState() -> void {
}

}

#endif
