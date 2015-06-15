#if defined(Hiro_VerticalScroller)

auto mVerticalScroller::allocate() -> pObject* {
  return new pVerticalScroller(*this);
}

//

auto mVerticalScroller::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mVerticalScroller::length() const -> unsigned {
  return state.length;
}

auto mVerticalScroller::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mVerticalScroller::position() const -> unsigned {
  return state.position;
}

auto mVerticalScroller::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mVerticalScroller::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

#endif
