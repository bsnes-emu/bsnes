void VerticalLayout::append(HorizontalLayout &layout, unsigned spacing) {
  children.append({ &layout, 0, 0, 0, spacing });
}

void VerticalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

Geometry VerticalLayout::minimumGeometry() {
  bool maximumWidth  = false;
  bool maximumHeight = false;

  unsigned width  = margin * 2;
  unsigned height = margin * 2;

  foreach(child, children) {
    if(child.width  == MaximumSize) maximumWidth  = true;
    if(child.height == MaximumSize) maximumHeight = true;

    if(child.width  != MaximumSize) width = max(width, child.width);
    if(child.height != MaximumSize) height += child.height;
  }

  return { 0, 0, maximumWidth ? MaximumSize : width, maximumHeight ? MaximumSize : height };
}

void VerticalLayout::setGeometry(const Geometry &containerGeometry) {
  setMinimumGeometry();
  setLayoutGeometry();

  Geometry geometry = containerGeometry;
  geometry.x      += margin;
  geometry.y      += margin;
  geometry.width  -= margin * 2;
  geometry.height -= margin * 2;

  Geometry layoutGeometry = geometry;
  auto children = this->children;

  unsigned minimumHeight = 0, maximumHeightCounter = 0;
  foreach(child, children) {
    if(child.height == MaximumSize) maximumHeightCounter++;
    if(child.height != MaximumSize) minimumHeight += child.height;
    minimumHeight += child.spacing;
  }

  foreach(child, children) {
    if(child.width  == MaximumSize) child.width  = geometry.width;
    if(child.height == MaximumSize) child.height = (geometry.height - minimumHeight) / maximumHeightCounter;
  }

  unsigned maximumWidth = 0;
  foreach(child, children) maximumWidth = max(maximumWidth, child.width);

  foreach(child, children) {
    unsigned pivot = 0;  //(maximumWidth - child.width) / 2;
    Geometry childGeometry = { geometry.x + pivot, geometry.y, child.width, child.height };

    if(child.layout) child.layout->setGeometry(childGeometry);
    if(child.widget) child.widget->setGeometry(childGeometry);

    geometry.y += child.height + child.spacing;
    geometry.height -= child.height + child.spacing;
  }
}

void VerticalLayout::setLayoutGeometry() {
  foreach(child, children) {
    if(child.layout) {
      child.width  = child.layout->minimumGeometry().width;
      child.height = child.layout->minimumGeometry().height;
    }
  }
}

void VerticalLayout::setMargin(unsigned margin) {
  this->margin = margin;
}

void VerticalLayout::setMinimumGeometry() {
  foreach(child, children) {
    if(child.layout) child.layout->setMinimumGeometry();
    if(child.widget) {
      Geometry minimumGeometry = child.widget->minimumGeometry();
      if(child.width  == MinimumSize) child.width  = minimumGeometry.width;
      if(child.height == MinimumSize) child.height = minimumGeometry.height;
    }
  }
}

void VerticalLayout::setParent(Window &parent) {
  foreach(child, children) {
    if(child.layout) child.layout->setParent(parent);
    if(child.widget) parent.append(*child.widget);
  }
}

void VerticalLayout::setVisible(bool visible) {
  foreach(child, children) {
    if(child.layout) child.layout->setVisible(visible);
    if(child.widget) child.widget->setVisible(visible);
  }
}

VerticalLayout::VerticalLayout() {
  margin = 0;
}
