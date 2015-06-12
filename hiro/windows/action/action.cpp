#if defined(Hiro_Action)

namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
  _synchronize();
}

auto pAction::setVisible(bool visible) -> void {
  _synchronize();
}

auto pAction::_parentMenu() -> maybe<pMenu&> {
  if(auto parent = self().parentMenu()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_parentMenuBar() -> maybe<pMenuBar&> {
  if(auto parent = self().parentMenuBar(true)) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_parentPopupMenu() -> maybe<pPopupMenu&> {
  if(auto parent = self().parentPopupMenu(true)) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pAction::_synchronize() -> void {
  if(auto parent = _parentMenuBar()) parent->_update();
}

}

#endif
