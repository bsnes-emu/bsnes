auto mProgressBar::allocate() -> pObject* {
  return new pProgressBar(*this);
}

//

auto mProgressBar::position() const -> unsigned {
  return state.position;
}

auto mProgressBar::setPosition(unsigned position) -> type& {
  state.position = position;
  signal(setPosition, position);
  return *this;
}
