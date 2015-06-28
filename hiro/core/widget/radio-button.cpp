#if defined(Hiro_RadioButton)

auto mRadioButton::allocate() -> pObject* {
  return new pRadioButton(*this);
}

//

auto mRadioButton::bordered() const -> bool {
  return state.bordered;
}

auto mRadioButton::checked() const -> bool {
  return state.checked;
}

auto mRadioButton::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mRadioButton::group() const -> Group {
  return state.group;
}

auto mRadioButton::icon() const -> image {
  return state.icon;
}

auto mRadioButton::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
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
  if(auto group = this->group()) {
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioButton = dynamic_cast<mRadioButton*>(object.data())) {
          radioButton->state.checked = false;
        }
      }
    }
  }
  state.checked = true;
  signal(setChecked);
  return *this;
}

auto mRadioButton::setGroup(sGroup group) -> type& {
  state.group = group;
  signal(setGroup, group);
  if(group && group->objects() == 1) setChecked();
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

#endif
