auto mRadioButton::allocate() -> pObject* {
  return new pRadioButton(*this);
}

//

auto mRadioButton::group(const vector<shared_pointer_weak<mRadioButton>>& group) -> void {
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

auto mRadioButton::bordered() const -> bool {
  return state.bordered;
}

auto mRadioButton::checked() const -> bool {
  return state.checked;
}

auto mRadioButton::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mRadioButton::icon() const -> image {
  return state.icon;
}

auto mRadioButton::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mRadioButton::orientation() const -> Orientation {
  return state.orientation;
}

auto mRadioButton::setBordered(bool bordered) -> type& {
  state.bordered = bordered;
  signal(setBordered, bordered);
  return *this;
}

auto mRadioButton::setChecked() -> type& {
  for(auto& weak : state.group) {
    if(auto item = weak.acquire()) item->state.checked = false;
  }
  state.checked = true;
  signal(setChecked);
  return *this;
}

auto mRadioButton::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mRadioButton::setOrientation(Orientation orientation) -> type& {
  state.orientation = orientation;
  signal(setOrientation, orientation);
  return *this;
}

auto mRadioButton::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mRadioButton::text() const -> string {
  return state.text;
}
