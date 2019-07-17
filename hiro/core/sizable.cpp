#if defined(Hiro_Sizable)

auto mSizable::allocate() -> pObject* {
  return new pSizable(*this);
}

auto mSizable::collapsible() const -> bool {
  return state.collapsible;
}

auto mSizable::doSize() const -> void {
  if(state.onSize) return state.onSize();
}

auto mSizable::geometry() const -> Geometry {
  return state.geometry;
}

auto mSizable::minimumSize() const -> Size {
  return signal(minimumSize);
}

auto mSizable::onSize(const function<void ()>& callback) -> type& {
  state.onSize = callback;
  return *this;
}

auto mSizable::setCollapsible(bool collapsible) -> type& {
  state.collapsible = collapsible;
  signal(setCollapsible, collapsible);
  return *this;
}

auto mSizable::setGeometry(Geometry geometry) -> type& {
  state.geometry = geometry;
  signal(setGeometry, geometry);
  return *this;
}

#endif
