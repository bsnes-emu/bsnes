auto mMenuCheckItem::allocate() -> pObject* {
  return new pMenuCheckItem(*this);
}

//

auto mMenuCheckItem::checked() const -> bool {
  return state.checked;
}

auto mMenuCheckItem::doToggle() const -> void {
  if(state.onToggle) return state.onToggle();
}

auto mMenuCheckItem::onToggle(const function<void ()>& function) -> type& {
  state.onToggle = function;
  return *this;
}

auto mMenuCheckItem::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
}

auto mMenuCheckItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mMenuCheckItem::text() const -> string {
  return state.text;
}
