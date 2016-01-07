#if defined(Hiro_MenuItem)

auto mMenuItem::allocate() -> pObject* {
  return new pMenuItem(*this);
}

//

auto mMenuItem::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mMenuItem::icon() const -> image {
  return state.icon;
}

auto mMenuItem::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mMenuItem::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mMenuItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mMenuItem::text() const -> string {
  return state.text;
}

#endif
