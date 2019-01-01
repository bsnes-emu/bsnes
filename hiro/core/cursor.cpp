#if defined(Hiro_Cursor)

Cursor::Cursor(int offset, int length) {
  setCursor(offset, length);
}

Cursor::operator bool() const {
  return offset() || length();
}

auto Cursor::operator==(const Cursor& source) const -> bool {
  return offset() == source.offset() && length() == source.length();
}

auto Cursor::operator!=(const Cursor& source) const -> bool {
  return !operator==(source);
}

auto Cursor::length() const -> int {
  return state.length;
}

auto Cursor::offset() const -> int {
  return state.offset;
}

auto Cursor::setCursor(int offset, int length) -> type& {
  state.offset = offset;
  state.length = length;
  return *this;
}

auto Cursor::setLength(int length) -> type& {
  state.length = length;
  return *this;
}

auto Cursor::setOffset(int offset) -> type& {
  state.offset = offset;
  return *this;
}

#endif
