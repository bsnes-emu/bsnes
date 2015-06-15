#if defined(Hiro_FixedLayout)

auto mFixedLayout::append(sSizable sizable, Geometry geometry) -> type& {
  properties.append({geometry});
  mLayout::append(sizable);
  sizable->setGeometry(geometry);
  return *this;
}

auto mFixedLayout::minimumSize() const -> Size {
  signed width = Size::Minimum, height = Size::Minimum;
  for(auto n : range(sizables())) {
    width  = max(width,  sizable(n)->minimumSize().width());
    height = max(height, sizable(n)->minimumSize().height());
  }
  return {width, height};
}

auto mFixedLayout::remove(sSizable sizable) -> type& {
  properties.remove(sizable->offset());
  mLayout::remove(sizable);
  return *this;
}

auto mFixedLayout::reset() -> type& {
  mLayout::reset();
  properties.reset();
  return *this;
}

auto mFixedLayout::setEnabled(bool enabled) -> type& {
  mLayout::setEnabled(enabled);
  for(auto n : range(sizables())) {
    sizable(n)->setEnabled(sizable(n)->enabled());
  }
  return *this;
}

auto mFixedLayout::setFont(const string& font) -> type& {
  mLayout::setFont(font);
  for(auto n : range(sizables())) {
    sizable(n)->setFont(sizable(n)->font());
  }
  return *this;
}

auto mFixedLayout::setVisible(bool visible) -> type& {
  mLayout::setVisible(visible);
  for(auto n : range(sizables())) {
    sizable(n)->setVisible(sizable(n)->visible());
  }
  return *this;
}

#endif
