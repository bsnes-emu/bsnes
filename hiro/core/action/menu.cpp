#if defined(Hiro_Menu)

auto mMenu::allocate() -> pObject* {
  return new pMenu(*this);
}

auto mMenu::destruct() -> void {
  for(auto& action : state.actions) action->destruct();
  mAction::destruct();
}

//

auto mMenu::action(unsigned position) const -> sAction {
  if(position >= actions()) throw;
  return state.actions[position];
}

auto mMenu::actions() const -> unsigned {
  return state.actions.size();
}

auto mMenu::append(sAction action) -> type& {
  state.actions.append(action);
  action->setParent(this, actions() - 1);
  signal(append, *action);
  return *this;
}

auto mMenu::icon() const -> image {
  return state.icon;
}

auto mMenu::remove(sAction action) -> type& {
  signal(remove, *action);
  state.actions.remove(action->offset());
  for(auto n : range(action->offset(), actions())) {
    state.actions[n]->offset(-1);
  }
  action->setParent();
}

auto mMenu::reset() -> type& {
  while(state.actions) remove(state.actions.last());
  return *this;
}

auto mMenu::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mMenu::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mMenu::text() const -> string {
  return state.text;
}

#endif
