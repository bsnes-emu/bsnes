#if defined(Hiro_SourceEdit)

auto mSourceEdit::allocate() -> pObject* {
  return new pSourceEdit(*this);
}

//

auto mSourceEdit::cursor() const -> Cursor {
  return signal(cursor);
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

auto mSourceEdit::language() const -> string {
  return state.language;
}

auto mSourceEdit::numbered() const -> bool {
  return state.numbered;
}

auto mSourceEdit::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mSourceEdit::onMove(const function<void ()>& callback) -> type& {
  state.onMove = callback;
  return *this;
}

auto mSourceEdit::scheme() const -> string {
  return state.scheme;
}

auto mSourceEdit::setCursor(Cursor cursor) -> type& {
  signal(setCursor, cursor);
  return *this;
}

auto mSourceEdit::setEditable(bool editable) -> type& {
  state.editable = editable;
  signal(setEditable, editable);
  return *this;
}

auto mSourceEdit::setLanguage(const string& language) -> type& {
  state.language = language;
  signal(setLanguage, language);
  return *this;
}

auto mSourceEdit::setNumbered(bool numbered) -> type& {
  state.numbered = numbered;
  signal(setNumbered, numbered);
  return *this;
}

auto mSourceEdit::setScheme(const string& scheme) -> type& {
  state.scheme = scheme;
  signal(setScheme, scheme);
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
