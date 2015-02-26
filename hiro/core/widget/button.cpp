auto mButton::allocate() -> pObject* {
  return new pButton(*this);
}

//

auto mButton::bordered() const -> bool {
  return state.bordered;
}

auto mButton::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mButton::icon() const -> image {
  return state.icon;
}

auto mButton::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mButton::orientation() const -> Orientation {
  return state.orientation;
}

auto mButton::setBordered(bool bordered) -> type& {
  state.bordered = bordered;
  signal(setBordered, bordered);
  return *this;
}

auto mButton::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mButton::setOrientation(Orientation orientation) -> type& {
  state.orientation = orientation;
  signal(setOrientation, orientation);
  return *this;
}

auto mButton::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mButton::text() const -> string {
  return state.text;
}
