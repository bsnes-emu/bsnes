#if defined(Hiro_MenuBar)

auto mMenuBar::allocate() -> pObject* {
  return new pMenuBar(*this);
}

auto mMenuBar::destruct() -> void {
  for(auto& menu : state.menus) menu->destruct();
  mObject::destruct();
}

//

auto mMenuBar::append(sMenu menu) -> type& {
  state.menus.append(menu);
  menu->setParent(this, menuCount() - 1);
  signal(append, menu);
  return *this;
}

auto mMenuBar::menu(unsigned position) const -> Menu {
  if(position < menuCount()) return state.menus[position];
  return {};
}

auto mMenuBar::menuCount() const -> unsigned {
  return state.menus.size();
}

auto mMenuBar::menus() const -> vector<Menu> {
  vector<Menu> menus;
  for(auto& menu : state.menus) menus.append(menu);
  return menus;
}

auto mMenuBar::remove() -> type& {
  if(auto window = parentWindow()) window->remove(window->menuBar());
  return *this;
}

auto mMenuBar::remove(sMenu menu) -> type& {
  signed offset = menu->offset();
  signal(remove, *menu);
  state.menus.remove(offset);
  for(auto n : range(offset, menuCount())) {
    state.menus[n]->adjustOffset(-1);
  }
  menu->setParent();
  return *this;
}

auto mMenuBar::reset() -> type& {
  while(state.menus) remove(state.menus.last());
  return *this;
}

auto mMenuBar::setParent(mObject* parent, signed offset) -> type& {
  for(auto n : rrange(state.menus)) state.menus[n]->destruct();
  mObject::setParent(parent, offset);
  for(auto& menu : state.menus) menu->setParent(this, menu->offset());
  return *this;
}

#endif
