#if defined(Hiro_VerticalSlider)

auto mVerticalSlider::allocate() -> pObject* {
  return new pVerticalSlider(*this);
}

//

auto mVerticalSlider::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mVerticalSlider::length() const -> unsigned {
  return state.length;
}

auto mVerticalSlider::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mVerticalSlider::position() const -> unsigned {
  return state.position;
}

auto mVerticalSlider::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mVerticalSlider::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}

#endif
