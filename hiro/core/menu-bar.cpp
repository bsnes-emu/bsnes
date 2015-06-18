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
  menu->setParent(this, menus() - 1);
  signal(append, menu);
  return *this;
}

auto mMenuBar::menu(unsigned position) const -> Menu {
  if(position < menus()) return state.menus[position];
  return {};
}

auto mMenuBar::menus() const -> unsigned {
  return state.menus.size();
}

auto mMenuBar::remove() -> type& {
  if(auto window = parentWindow()) window->remove(window->menuBar());
  return *this;
}

auto mMenuBar::remove(sMenu menu) -> type& {
  signed offset = menu->offset();
  signal(remove, *menu);
  state.menus.remove(offset);
  for(auto n : range(offset, menus())) {
    state.menus[n]->adjustOffset(-1);
  }
  menu->setParent();
  return *this;
}

auto mMenuBar::reset() -> type& {
  while(state.menus) remove(state.menus.last());
  return *this;
}

#endif
