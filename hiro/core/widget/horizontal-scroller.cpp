#if defined(Hiro_HorizontalScroller)

auto mHorizontalScroller::allocate() -> pObject* {
  return new pHorizontalScroller(*this);
}

//

auto mHorizontalScroller::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mHorizontalScroller::length() const -> unsigned {
  return state.length;
}

auto mHorizontalScroller::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mHorizontalScroller::position() const -> unsigned {
  return state.position;
}

auto mHorizontalScroller::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mHorizontalScroller::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

#endif
