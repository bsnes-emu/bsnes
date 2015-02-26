auto mHorizontalSlider::allocate() -> pObject* {
  return new pHorizontalSlider(*this);
}

//

auto mHorizontalSlider::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mHorizontalSlider::length() const -> unsigned {
  return state.length;
}

auto mHorizontalSlider::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mHorizontalSlider::position() const -> unsigned {
  return state.position;
}

auto mHorizontalSlider::setLength(unsigned length) -> type& {
  state.length = length;
  signal(setLength, length);
  return *this;
}

auto mHorizontalSlider::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}
