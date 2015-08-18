#if defined(Hiro_PopupMenu)

namespace hiro {

auto pPopupMenu::construct() -> void {
  qtPopupMenu = new QMenu;
  _setState();
}

auto pPopupMenu::destruct() -> void {
  delete qtPopupMenu;
  qtPopupMenu = nullptr;
}

auto pPopupMenu::append(sAction action) -> void {
  _setState();
}

auto pPopupMenu::remove(sAction action) -> void {
  _setState();
}

auto pPopupMenu::setFont(const string& font) -> void {
  _setState();
}

auto pPopupMenu::setVisible(bool visible) -> void {
  if(visible) qtPopupMenu->popup(QCursor::pos());
}

auto pPopupMenu::_setState() -> void {
  qtPopupMenu->setFont(pFont::create(self().font(true)));
  for(auto& action : state().actions) {
    if(auto self = action->self()) self->_setState();
  }
}

}

#endif
