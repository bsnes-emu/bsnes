#if defined(Hiro_Menu)

auto mMenu::allocate() -> pObject* {
  return new pMenu(*this);
}

auto mMenu::destruct() -> void {
  for(auto& action : state.actions) action->destruct();
  mAction::destruct();
}

//

auto mMenu::action(unsigned position) const -> Action {
  if(position < actionCount()) return state.actions[position];
  return {};
}

auto mMenu::actionCount() const -> unsigned {
  return state.actions.size();
}

auto mMenu::actions() const -> vector<Action> {
  vector<Action> actions;
  for(auto& action : state.actions) actions.append(action);
  return actions;
}

auto mMenu::append(sAction action) -> type& {
  state.actions.append(action);
  action->setParent(this, actionCount() - 1);
  signal(append, *action);
  return *this;
}

auto mMenu::icon() const -> image {
  return state.icon;
}

auto mMenu::remove(sAction action) -> type& {
  signal(remove, *action);
  state.actions.remove(action->offset());
  for(auto n : range(action->offset(), actionCount())) {
    state.actions[n]->adjustOffset(-1);
  }
  action->setParent();
  return *this;
}

auto mMenu::reset() -> type& {
  while(state.actions) remove(state.actions.right());
  return *this;
}

auto mMenu::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mMenu::setParent(mObject* parent, signed offset) -> type& {
  for(auto& action : reverse(state.actions)) action->destruct();
  mObject::setParent(parent, offset);
  for(auto& action : state.actions) action->setParent(this, action->offset());
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
