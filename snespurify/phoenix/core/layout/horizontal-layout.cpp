void HorizontalLayout::append(VerticalLayout &layout, unsigned spacing) {
  children.append({ &layout, 0, MinimumSize, MinimumSize, spacing });
}

void HorizontalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

Geometry HorizontalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  foreach(child, children) {
    width += child.spacing;
    if(child.width == MinimumSize || child.width == MaximumSize) {
      if(child.layout) width += child.layout->minimumGeometry().width;
      if(child.widget) width += child.widget->minimumGeometry().width;
      continue;
    }
    width += child.width;
  }

  foreach(child, children) {
    if(child.height == MinimumSize || child.height == MaximumSize) {
      if(child.layout) height = max(height, child.layout->minimumGeometry().height);
      if(child.widget) height = max(height, child.widget->minimumGeometry().height);
      continue;
    }
    height = max(height, child.height);
  }

  return { 0, 0, margin * 2 + width, margin * 2 + height };
}

Geometry HorizontalLayout::minimumLayoutGeometry() {
  unsigned width = 0, height = 0;
  bool maximumWidth  = false;
  bool maximumHeight = false;

  foreach(child, children) {
    if(child.width == MaximumSize) {
      maximumWidth = true;
      break;
    }

    if(child.width == MinimumSize) {
      if(child.layout) width += child.layout->minimumGeometry().width;
      if(child.widget) width += child.widget->minimumGeometry().width;
      continue;
    }

    width += child.width;
  }

  foreach(child, children) {
    if(child.height == MaximumSize) {
      maximumHeight = true;
      break;
    }

    if(child.height == MinimumSize) {
      if(child.layout) height = max(height, child.layout->minimumGeometry().height);
      if(child.widget) height = max(height, child.widget->minimumGeometry().height);
      continue;
    }

    height = max(height, child.height);
  }

  return { 0, 0, maximumWidth ? MaximumSize : margin * 2 + width, maximumHeight ? MaximumSize : margin * 2 + height };
}

void HorizontalLayout::setGeometry(const Geometry &containerGeometry) {
  auto children = this->children;
  foreach(child, children) {
    if(child.layout) {
      child.width  = child.layout->minimumLayoutGeometry().width;
      child.height = child.layout->minimumLayoutGeometry().height;
    }

    if(child.widget) {
      if(child.width  == MinimumSize) child.width  = child.widget->minimumGeometry().width;
      if(child.height == MinimumSize) child.height = child.widget->minimumGeometry().height;
    }
  }

  Geometry geometry = containerGeometry;
  geometry.x      += margin;
  geometry.y      += margin;
  geometry.width  -= margin * 2;
  geometry.height -= margin * 2;

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

void HorizontalLayout::setMargin(unsigned margin) {
  this->margin = margin;
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
