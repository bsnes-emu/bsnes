#if defined(Hiro_TextCursor)

TextCursor::TextCursor(int offset, int length) {
  setTextCursor(offset, length);
}

TextCursor::operator bool() const {
  return offset() || length();
}

auto TextCursor::operator==(const TextCursor& source) const -> bool {
  return offset() == source.offset() && length() == source.length();
}

auto TextCursor::operator!=(const TextCursor& source) const -> bool {
  return !operator==(source);
}

auto TextCursor::length() const -> int {
  return state.length;
}

auto TextCursor::offset() const -> int {
  return state.offset;
}

auto TextCursor::setLength(int length) -> type& {
  state.length = length;
  return *this;
}

auto TextCursor::setOffset(int offset) -> type& {
  state.offset = offset;
  return *this;
}

auto TextCursor::setTextCursor(int offset, int length) -> type& {
  state.offset = offset;
  state.length = length;
  return *this;
}

#endif
