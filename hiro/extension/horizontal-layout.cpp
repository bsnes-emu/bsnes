#if defined(Hiro_HorizontalLayout)

auto mHorizontalLayout::append(sSizable sizable, Size size, signed spacing) -> type& {
  properties.append({size.width(), size.height(), spacing < 0 ? settings.spacing : spacing});
  mLayout::append(sizable);
  return *this;
}

auto mHorizontalLayout::modify(sSizable sizable, Size size, signed spacing) -> type& {
  if(sizable && this->sizable(sizable->offset()) == sizable) {
    auto& properties = this->properties[sizable->offset()];
    properties.width = size.width();
    properties.height = size.height();
    properties.spacing = spacing;
  }
  return *this;
}

auto mHorizontalLayout::minimumSize() const -> Size {
  signed width = 0, height = 0;

  for(auto n : range(sizableCount())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.width == Size::Minimum || child.width == Size::Maximum) {
      width += sizable(n)->minimumSize().width();
    } else {
      width += child.width;
    }
    if(&child != &properties.last()) width += child.spacing;
  }

  for(auto n : range(sizableCount())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.height == Size::Minimum || child.height == Size::Maximum) {
      height = max(height, sizable(n)->minimumSize().height());
      continue;
    }
    height = max(height, child.height);
  }

  return {settings.margin * 2 + width, settings.margin * 2 + height};
}

auto mHorizontalLayout::remove(sSizable sizable) -> type& {
  properties.remove(sizable->offset());
  mLayout::remove(sizable);
  return *this;
}

auto mHorizontalLayout::reset() -> type& {
  mLayout::reset();
  properties.reset();
  return *this;
}

auto mHorizontalLayout::setAlignment(double alignment) -> type& {
  settings.alignment = max(0.0, min(1.0, alignment));
  return *this;
}

auto mHorizontalLayout::setEnabled(bool enabled) -> type& {
  mLayout::setEnabled(enabled);
  for(auto n : range(sizableCount())) {
    sizable(n)->setEnabled(sizable(n)->enabled());
  }
  return *this;
}

auto mHorizontalLayout::setFont(const Font& font) -> type& {
  mLayout::setFont(font);
  for(auto n : range(sizableCount())) {
    sizable(n)->setFont(sizable(n)->font());
  }
  return *this;
}

auto mHorizontalLayout::setGeometry(Geometry containerGeometry) -> type& {
  mLayout::setGeometry(containerGeometry);

  auto properties = this->properties;
  for(auto n : range(sizableCount())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.width  == Size::Minimum) child.width  = sizable(n)->minimumSize().width();
    if(child.height == Size::Minimum) child.height = sizable(n)->minimumSize().height();
  }

  Geometry geometry = containerGeometry;
  geometry.setX     (geometry.x()      + settings.margin    );
  geometry.setY     (geometry.y()      + settings.margin    );
  geometry.setWidth (geometry.width()  - settings.margin * 2);
  geometry.setHeight(geometry.height() - settings.margin * 2);

  signed minimumWidth = 0, maximumWidthCounter = 0;
  for(auto& child : properties) {
    if(child.width == Size::Maximum) maximumWidthCounter++;
    if(child.width != Size::Maximum) minimumWidth += child.width;
    if(&child != &properties.last()) minimumWidth += child.spacing;
  }

  for(auto& child : properties) {
    if(child.width  == Size::Maximum) child.width  = (geometry.width() - minimumWidth) / maximumWidthCounter;
    if(child.height == Size::Maximum) child.height = geometry.height();
  }

  signed maximumHeight = 0;
  for(auto& child : properties) maximumHeight = max(maximumHeight, child.height);

  for(auto n : range(sizableCount())) {
    auto& child = properties[sizable(n)->offset()];
    signed pivot = (maximumHeight - child.height) * settings.alignment;
    Geometry childGeometry = {geometry.x(), geometry.y() + pivot, child.width, child.height};
    if(childGeometry.width()  < 1) childGeometry.setWidth (1);
    if(childGeometry.height() < 1) childGeometry.setHeight(1);
    sizable(n)->setGeometry(childGeometry);

    geometry.setX    (geometry.x()     + child.width + child.spacing);
    geometry.setWidth(geometry.width() - child.width + child.spacing);
  }
}

auto mHorizontalLayout::setMargin(signed margin) -> type& {
  settings.margin = margin;
  return *this;
}

auto mHorizontalLayout::setSpacing(signed spacing) -> type& {
  settings.spacing = spacing;
  return *this;
}

auto mHorizontalLayout::setVisible(bool visible) -> type& {
  mLayout::setVisible(visible);
  for(auto n : range(sizableCount())) {
    sizable(n)->setVisible(sizable(n)->visible());
  }
  return *this;
}

#endif
