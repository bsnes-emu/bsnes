#if defined(Hiro_Action)

namespace hiro {

auto pAction::construct() -> void {
}

auto pAction::destruct() -> void {
}

auto pAction::setEnabled(bool enabled) -> void {
/*
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->setEnabled(enabled);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setEnabled(enabled);
  }
*/
}

auto pAction::setFont(const string& font) -> void {
/*
  QFont qtFont = pFont::create(font);

  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.setFont(font);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setFont(qtFont);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setFont(qtFont);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setFont(qtFont);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setFont(qtFont);
  }
*/
}

auto pAction::setVisible(bool visible) -> void {
/*
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->menuAction()->setVisible(visible);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setVisible(visible);
  }
*/
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
