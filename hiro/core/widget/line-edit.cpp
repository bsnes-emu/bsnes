#if defined(Hiro_LineEdit)

auto mLineEdit::allocate() -> pObject* {
  return new pLineEdit(*this);
}

//

auto mLineEdit::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mLineEdit::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mLineEdit::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mLineEdit::editable() const -> bool {
  return state.editable;
}

auto mLineEdit::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mLineEdit::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mLineEdit::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mLineEdit::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mLineEdit::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mLineEdit::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mLineEdit::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mLineEdit::text() const -> string {
  return state.text;
}

#endif
