#if defined(Hiro_MenuItem)

namespace hiro {

auto pMenuItem::construct() -> void {
  qtMenuItem = new QtMenuItem(*this);
  qtMenuItem->connect(qtMenuItem, SIGNAL(triggered()), SLOT(onActivate()));

  if(auto parent = _parentMenu()) {
    parent->qtMenu->addAction(qtMenuItem);
  }

  if(auto parent = _parentPopupMenu()) {
    parent->qtPopupMenu->addAction(qtMenuItem);
  }

  _setState();
}

auto pMenuItem::destruct() -> void {
  delete qtMenuItem;
  qtMenuItem = nullptr;
}

auto pMenuItem::setIcon(const image& icon) -> void {
  _setState();
}

auto pMenuItem::setText(const string& text) -> void {
  _setState();
}

auto pMenuItem::_setState() -> void {
  qtMenuItem->setIcon(CreateIcon(state().icon));
  qtMenuItem->setText(QString::fromUtf8(state().text));
}

auto QtMenuItem::onActivate() -> void {
  p.self().doActivate();
}

}

#endif
