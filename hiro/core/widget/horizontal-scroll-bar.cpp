#if defined(Hiro_HorizontalScrollBar)

auto mHorizontalScrollBar::allocate() -> pObject* {
  return new pHorizontalScrollBar(*this);
}

//

auto mHorizontalScrollBar::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mHorizontalScrollBar::length() const -> unsigned {
  return state.length;
}

auto mHorizontalScrollBar::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mHorizontalScrollBar::position() const -> unsigned {
  return state.position;
}

auto mHorizontalScrollBar::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mHorizontalScrollBar::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

#endif
