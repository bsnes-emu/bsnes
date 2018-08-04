#if defined(Hiro_SourceEdit)

auto mSourceEdit::allocate() -> pObject* {
  return new pSourceEdit(*this);
}

//

auto mSourceEdit::cursor() const -> Cursor {
  return state.cursor;
}

auto mSourceEdit::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mSourceEdit::doMove() const -> void {
  if(state.onMove) return state.onMove();
}

auto mSourceEdit::editable() const -> bool {
  return state.editable;
}

auto mSourceEdit::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mSourceEdit::onMove(const function<void ()>& callback) -> type& {
  state.onMove = callback;
  return *this;
}

auto mSourceEdit::setCursor(Cursor cursor) -> type& {
  state.cursor = cursor;
  signal(setCursor, cursor);
  return *this;
}

auto mSourceEdit::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mSourceEdit::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mSourceEdit::setWordWrap(bool wordWrap) -> type& {
  state.wordWrap = wordWrap;
  signal(setWordWrap, wordWrap);
  return *this;
}

auto mSourceEdit::text() const -> string {
  return signal(text);
}

auto mSourceEdit::wordWrap() const -> bool {
  return state.wordWrap;
}

#endif
