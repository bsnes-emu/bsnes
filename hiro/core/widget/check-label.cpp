#if defined(Hiro_CheckLabel)

auto mCheckLabel::allocate() -> pObject* {
  return new pCheckLabel(*this);
}

//

auto mCheckLabel::checked() const -> bool {
  return state.checked;
}

auto mCheckLabel::doToggle() const -> void {
  if(state.onToggle) return state.onToggle();
}

auto mCheckLabel::onToggle(const function<void ()>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mCheckLabel::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mCheckLabel::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mCheckLabel::text() const -> string {
  return state.text;
}

#endif
