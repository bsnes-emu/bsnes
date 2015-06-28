#if defined(Hiro_MenuRadioItem)

auto mMenuRadioItem::allocate() -> pObject* {
  return new pMenuRadioItem(*this);
}

//

auto mMenuRadioItem::checked() const -> bool {
  return state.checked;
}

auto mMenuRadioItem::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mMenuRadioItem::group() const -> Group {
  return state.group;
}

auto mMenuRadioItem::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mMenuRadioItem::setChecked() -> type& {
  if(auto group = this->group()) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto menuRadioItem = dynamic_cast<mMenuRadioItem*>(object.data())) {
          menuRadioItem->state.checked = false;
        }
      }
    }
  }
  state.checked = true;
  signal(setChecked);
  return *this;
}

auto mMenuRadioItem::setGroup(sGroup group) -> type& {
  state.group = group;
  signal(setGroup, group);
  if(group && group->objects() == 1) setChecked();
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
