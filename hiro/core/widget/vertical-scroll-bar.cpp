#if defined(Hiro_VerticalScrollBar)

auto mVerticalScrollBar::allocate() -> pObject* {
  return new pVerticalScrollBar(*this);
}

//

auto mVerticalScrollBar::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mVerticalScrollBar::length() const -> unsigned {
  return state.length;
}

auto mVerticalScrollBar::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mVerticalScrollBar::position() const -> unsigned {
  return state.position;
}

auto mVerticalScrollBar::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mVerticalScrollBar::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

#endif
