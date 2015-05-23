#if defined(Hiro_MenuRadioItem)

auto mMenuRadioItem::allocate() -> pObject* {
  return new pMenuRadioItem(*this);
}

//

auto mMenuRadioItem::group(const vector<wMenuRadioItem>& group) -> void {
  for(auto& weak : group) {
    if(auto item = weak.acquire()) item->state.group = group;
  }
  for(auto& weak : group) {
    if(auto item = weak.acquire()) {
      if(item->self()) item->self()->setGroup(group);
    }
  }
  if(group.size()) {
    if(auto item = group.first().acquire()) item->setChecked();
  }
}

auto mMenuRadioItem::checked() const -> bool {
  return state.checked;
}

auto mMenuRadioItem::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mMenuRadioItem::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mMenuRadioItem::setChecked() -> type& {
  for(auto& weak : state.group) {
    if(auto item = weak.acquire()) item->state.checked = false;
  }
  state.checked = true;
  signal(setChecked);
  return *this;
}

auto mMenuRadioItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mMenuRadioItem::text() const -> string {
  return state.text;
}

#endif
