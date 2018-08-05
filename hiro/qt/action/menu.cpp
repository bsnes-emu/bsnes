#if defined(Hiro_Menu)

namespace hiro {

auto pMenu::construct() -> void {
  qtMenu = new QMenu;

  if(auto parent = _parentMenu()) {
    parent->qtMenu->addMenu(qtMenu);
  }

  if(auto parent = _parentMenuBar()) {
    if(auto window = parent->_parent()) {
      window->qtMenuBar->addMenu(qtMenu);
    }
  }

  if(auto parent = _parentPopupMenu()) {
    parent->qtPopupMenu->addMenu(qtMenu);
  }

  _setState();
}

auto pMenu::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtMenu;
  qtMenu = nullptr;
}

auto pMenu::append(sAction action) -> void {
}

auto pMenu::remove(sAction action) -> void {
}

auto pMenu::setIcon(const image& icon) -> void {
  _setState();
}

auto pMenu::setText(const string& text) -> void {
  _setState();
}

auto pMenu::_setState() -> void {
  qtMenu->setEnabled(self().enabled());
  qtMenu->setFont(pFont::create(self().font(true)));
  qtMenu->setIcon(CreateIcon(state().icon));
  qtMenu->setTitle(QString::fromUtf8(state().text));
  qtMenu->menuAction()->setVisible(self().visible());

  for(auto& action : state().actions) {
    if(auto self = action->self()) self->setFont(action->font(true));
  }
}

}

#endif
