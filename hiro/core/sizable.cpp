auto mSizable::allocate() -> pObject* {
  return new pSizable(*this);
}

auto mSizable::geometry() const -> Geometry {
  return state.geometry;
}

auto mSizable::minimumSize() const -> Size {
  return signal(minimumSize);
}

auto mSizable::setGeometry(Geometry geometry) -> type& {
  state.geometry = geometry;
  signal(setGeometry, geometry);
  return *this;
}
