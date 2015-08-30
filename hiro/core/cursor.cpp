#if defined(Hiro_Cursor)

Cursor::Cursor(signed offset, signed length) {
  setCursor(offset, length);
}

Cursor::operator bool() const {
  return offset() && length();
}

auto Cursor::operator==(const Cursor& source) const -> bool {
  return offset() == source.offset() && length() == source.length();
}

auto Cursor::operator!=(const Cursor& source) const -> bool {
  return !operator==(source);
}

auto Cursor::length() const -> signed {
  return state.length;
}

auto Cursor::offset() const -> signed {
  return state.offset;
}

auto Cursor::setCursor(signed offset, signed length) -> type& {
  state.offset = offset;
  state.length = length;
  return *this;
}

auto Cursor::setOffset(signed offset) -> type& {
  state.offset = offset;
  return *this;
}

auto Cursor::setLength(signed length) -> type& {
  state.length = length;
  return *this;
}

#endif
