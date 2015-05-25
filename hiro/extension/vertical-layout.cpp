#if defined(Hiro_VerticalLayout)

auto mVerticalLayout::append(shared_pointer<mSizable> sizable, Size size, signed spacing) -> type& {
  properties.append({size.width(), size.height(), spacing < 0 ? settings.spacing : spacing});
  mLayout::append(sizable);
  return *this;
}

auto mVerticalLayout::minimumSize() const -> Size {
  signed width = 0, height = 0;

  for(auto n : range(sizables())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.width == Size::Minimum || child.width == Size::Maximum) {
      width = max(width, sizable(n)->minimumSize().width());
      continue;
    }
    width = max(width, child.width);
  }

  for(auto n : range(sizables())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.height == Size::Minimum || child.height == Size::Maximum) {
      height += sizable(n)->minimumSize().height();
    } else {
      height += child.height;
    }
    if(&child != &properties.last()) height += child.spacing;
  }

  return {settings.margin * 2 + width, settings.margin * 2 + height};
}

auto mVerticalLayout::remove(shared_pointer<mSizable> sizable) -> type& {
  properties.remove(sizable->offset());
  mLayout::remove(sizable);
  return *this;
}

auto mVerticalLayout::reset() -> type& {
  mLayout::reset();
  properties.reset();
  return *this;
}

auto mVerticalLayout::setAlignment(double alignment) -> type& {
  settings.alignment = max(0.0, min(1.0, alignment));
  return *this;
}

auto mVerticalLayout::setEnabled(bool enabled) -> type& {
  mLayout::setEnabled(enabled);
  for(auto n : range(sizables())) {
    sizable(n)->setEnabled(sizable(n)->enabled());
  }
  return *this;
}

auto mVerticalLayout::setFont(const string& font) -> type& {
  mLayout::setFont(font);
  for(auto n : range(sizables())) {
    sizable(n)->setFont(sizable(n)->font());
  }
  return *this;
}

auto mVerticalLayout::setGeometry(Geometry containerGeometry) -> type& {
  mLayout::setGeometry(containerGeometry);

  auto properties = this->properties;
  for(auto n : range(sizables())) {
    auto& child = properties[sizable(n)->offset()];
    if(child.width  == Size::Minimum) child.width  = sizable(n)->minimumSize().width();
    if(child.height == Size::Minimum) child.height = sizable(n)->minimumSize().height();
  }

  Geometry geometry = containerGeometry;
  geometry.setX     (geometry.x()      + settings.margin    );
  geometry.setY     (geometry.y()      + settings.margin    );
  geometry.setWidth (geometry.width()  - settings.margin * 2);
  geometry.setHeight(geometry.height() - settings.margin * 2);

  signed minimumHeight = 0, maximumHeightCounter = 0;
  for(auto& child : properties) {
    if(child.height == Size::Maximum) maximumHeightCounter++;
    if(child.height != Size::Maximum) minimumHeight += child.height;
    if(&child != &properties.last()) minimumHeight += child.spacing;
  }

  for(auto& child : properties) {
    if(child.width  == Size::Maximum) child.width  = geometry.width();
    if(child.height == Size::Maximum) child.height = (geometry.height() - minimumHeight) / maximumHeightCounter;
  }

  signed maximumWidth = 0;
  for(auto& child : properties) maximumWidth = max(maximumWidth, child.width);

  for(auto n : range(sizables())) {
    auto& child = properties[sizable(n)->offset()];
    signed pivot = (maximumWidth - child.width) * settings.alignment;
    Geometry childGeometry = {geometry.x() + pivot, geometry.y(), child.width, child.height};
    if(childGeometry.width()  < 1) childGeometry.setWidth (1);
    if(childGeometry.height() < 1) childGeometry.setHeight(1);
    sizable(n)->setGeometry(childGeometry);

    geometry.setY     (geometry.y()      + child.height + child.spacing);
    geometry.setHeight(geometry.height() - child.height + child.spacing);
  }

  return *this;
}

auto mVerticalLayout::setMargin(signed margin) -> type& {
  settings.margin = margin;
  return *this;
}

auto mVerticalLayout::setSpacing(signed spacing) -> type& {
  settings.spacing = spacing;
  return *this;
}

auto mVerticalLayout::setVisible(bool visible) -> type& {
  mLayout::setVisible(visible);
  for(auto n : range(sizables())) {
    sizable(n)->setVisible(sizable(n)->visible());
  }
  return *this;
}

#endif
