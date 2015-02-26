auto mHexEdit::allocate() -> pObject* {
  return new pHexEdit(*this);
}

//

auto mHexEdit::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mHexEdit::columns() const -> unsigned {
  return state.columns;
}

auto mHexEdit::doRead(unsigned offset) const -> uint8_t {
  if(state.onRead) return state.onRead(offset);
  return 0x00;
}

auto mHexEdit::doWrite(unsigned offset, uint8_t data) const -> void {
  if(state.onWrite) return state.onWrite(offset, data);
}

auto mHexEdit::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mHexEdit::length() const -> unsigned {
  return state.length;
}

auto mHexEdit::offset() const -> unsigned {
  return state.offset;
}

auto mHexEdit::onRead(const function<uint8_t (unsigned)>& function) -> type& {
  state.onRead = function;
  return *this;
}

auto mHexEdit::onWrite(const function<void (unsigned, uint8_t)>& function) -> type& {
  state.onWrite = function;
  return *this;
}

auto mHexEdit::rows() const -> unsigned {
  return state.rows;
}

auto mHexEdit::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mHexEdit::setColumns(unsigned columns) -> type& {
  state.columns = columns;
  signal(setColumns, columns);
  return *this;
}

auto mHexEdit::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mHexEdit::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mHexEdit::setOffset(unsigned offset) -> type& {
  state.offset = offset;
  signal(setOffset, offset);
  return *this;
}

auto mHexEdit::setRows(unsigned rows) -> type& {
  state.rows = rows;
  signal(setRows, rows);
  return *this;
}

auto mHexEdit::update() -> type& {
  signal(update);
  return *this;
}
