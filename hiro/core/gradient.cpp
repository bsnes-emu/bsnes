#if defined(Hiro_Gradient)

Gradient::Gradient() {
}

Gradient::operator bool() const {
  return state.colors.size() == 4;
}

auto Gradient::operator==(const Gradient& source) const -> bool {
  if(state.colors.size() != source.state.colors.size()) return false;
  for(auto n : range(state.colors.size())) {
    if(state.colors[n] != source.state.colors[n]) return false;
  }
  return true;
}

auto Gradient::operator!=(const Gradient& source) const -> bool {
  return !operator==(source);
}

auto Gradient::setBilinear(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> type& {
  state.colors = {topLeft, topRight, bottomLeft, bottomRight};
  return *this;
}

auto Gradient::setHorizontal(Color left, Color right) -> type& {
  state.colors = {left, right, left, right};
  return *this;
}

auto Gradient::setVertical(Color top, Color bottom) -> type& {
  state.colors = {top, top, bottom, bottom};
  return *this;
}

#endif
