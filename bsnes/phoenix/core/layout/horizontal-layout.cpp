void HorizontalLayout::append(VerticalLayout &layout, unsigned spacing) {
  children.append({ &layout, 0, 0, 0, spacing });
}

void HorizontalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

Geometry HorizontalLayout::minimumGeometry() {
  bool maximumWidth  = false;
  bool maximumHeight = false;

  unsigned width  = margin * 2;
  unsigned height = margin * 2;

  foreach(child, children) {
    if(child.width  == MaximumSize) maximumWidth  = true;
    if(child.height == MaximumSize) maximumHeight = true;

    if(child.width  != MaximumSize) width += child.width;
    if(child.height != MaximumSize) height = max(height, child.height);
  }

  return { 0, 0, maximumWidth ? MaximumSize : width, maximumHeight ? MaximumSize : height };
}

void HorizontalLayout::setGeometry(const Geometry &containerGeometry) {
  setMinimumGeometry();
  setLayoutGeometry();

  Geometry geometry = containerGeometry;
  geometry.x      += margin;
  geometry.y      += margin;
  geometry.width  -= margin * 2;
  geometry.height -= margin * 2;

  Geometry layoutGeometry = geometry;
  auto children = this->children;

  unsigned minimumWidth = 0, maximumWidthCounter = 0;
  foreach(child, children) {
    if(child.width == MaximumSize) maximumWidthCounter++;
    if(child.width != MaximumSize) minimumWidth += child.width;
    minimumWidth += child.spacing;
  }

  foreach(child, children) {
    if(child.width  == MaximumSize) child.width  = (geometry.width - minimumWidth) / maximumWidthCounter;
    if(child.height == MaximumSize) child.height = geometry.height;
  }

  unsigned maximumHeight = 0;
  foreach(child, children) maximumHeight = max(maximumHeight, child.height);

  foreach(child, children) {
    unsigned pivot = (maximumHeight - child.height) / 2;
    Geometry childGeometry = { geometry.x, geometry.y + pivot, child.width, child.height };

    if(child.layout) child.layout->setGeometry(childGeometry);
    if(child.widget) child.widget->setGeometry(childGeometry);

    geometry.x += child.width + child.spacing;
    geometry.width -= child.width + child.spacing;
  }
}

void HorizontalLayout::setLayoutGeometry() {
  foreach(child, children) {
    if(child.layout) {
      child.width  = child.layout->minimumGeometry().width;
      child.height = child.layout->minimumGeometry().height;
    }
  }
}

void HorizontalLayout::setMargin(unsigned margin) {
  this->margin = margin;
}

void HorizontalLayout::setMinimumGeometry() {
  foreach(child, children) {
    if(child.layout) child.layout->setMinimumGeometry();
    if(child.widget) {
      Geometry minimumGeometry = child.widget->minimumGeometry();
      if(child.width  == MinimumSize) child.width  = minimumGeometry.width;
      if(child.height == MinimumSize) child.height = minimumGeometry.height;
    }
  }
}

void HorizontalLayout::setParent(Window &parent) {
  foreach(child, children) {
    if(child.layout) child.layout->setParent(parent);
    if(child.widget) parent.append(*child.widget);
  }
}

void HorizontalLayout::setVisible(bool visible) {
  foreach(child, children) {
    if(child.layout) child.layout->setVisible(visible);
    if(child.widget) child.widget->setVisible(visible);
  }
}

HorizontalLayout::HorizontalLayout() {
  margin = 0;
}
