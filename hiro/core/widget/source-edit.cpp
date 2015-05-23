#if defined(Hiro_SourceEdit)

auto mSourceEdit::allocate() -> pObject* {
  return new pSourceEdit(*this);
}

//

auto mSourceEdit::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mSourceEdit::doMove() const -> void {
  if(state.onMove) return state.onMove();
}

auto mSourceEdit::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mSourceEdit::onMove(const function<void ()>& function) -> type& {
  state.onMove = function;
  return *this;
}

auto mSourceEdit::position() const -> unsigned {
  return state.position;
}

auto mSourceEdit::setPosition(signed position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

auto mSourceEdit::setSelected(Position selected) -> type& {
  if(selected.x() < 0) return *this;
  if(selected.y() < 0) selected.setY(-1);
  else if(selected.x() > selected.y()) return *this;
  state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

auto mSourceEdit::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mSourceEdit::text() const -> string {
  return signal(text);
}

#endif
