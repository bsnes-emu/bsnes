#if defined(Hiro_TextEdit)

auto mTextEdit::allocate() -> pObject* {
  return new pTextEdit(*this);
}

//

auto mTextEdit::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mTextEdit::cursorPosition() const -> unsigned {
  return state.cursorPosition;
}

auto mTextEdit::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mTextEdit::doMove() const -> void {
  if(state.onMove) return state.onMove();
}

auto mTextEdit::editable() const -> bool {
  return state.editable;
}

auto mTextEdit::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mTextEdit::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mTextEdit::onMove(const function<void ()>& function) -> type& {
  state.onMove = function;
  return *this;
}

auto mTextEdit::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTextEdit::setCursorPosition(unsigned position) -> type& {
  state.cursorPosition = position;
  signal(setCursorPosition, position);
  return *this;
}

auto mTextEdit::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mTextEdit::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTextEdit::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTextEdit::setWordWrap(bool wordWrap) -> type& {
  state.wordWrap = wordWrap;
  signal(setWordWrap, wordWrap);
  return *this;
}

auto mTextEdit::text() const -> string {
  return signal(text);
}

auto mTextEdit::wordWrap() const -> bool {
  return state.wordWrap;
}

#endif
