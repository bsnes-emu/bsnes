void VerticalLayout::append(HorizontalLayout &layout, unsigned spacing) {
  children.append({ &layout, 0, MinimumSize, MinimumSize, spacing });
}

void VerticalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

Geometry VerticalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  foreach(child, children) {
    if(child.width == MinimumSize || child.width == MaximumSize) {
      if(child.layout) width = max(width, child.layout->minimumGeometry().width);
      if(child.widget) width = max(width, child.widget->minimumGeometry().width);
      continue;
    }
    width = max(width, child.width);
  }

  foreach(child, children) {
    height += child.spacing;
    if(child.height == MinimumSize || child.height == MaximumSize) {
      if(child.layout) height += child.layout->minimumGeometry().height;
      if(child.widget) height += child.widget->minimumGeometry().height;
      continue;
    }
    height += child.height;
  }

  return { 0, 0, margin * 2 + width, margin * 2 + height };
}

Geometry VerticalLayout::minimumLayoutGeometry() {
  unsigned width = 0, height = 0;
  bool maximumWidth  = false;
  bool maximumHeight = false;

  foreach(child, children) {
    if(child.width == MaximumSize) {
      maximumWidth = true;
      break;
    }

    if(child.width == MinimumSize) {
      if(child.layout) width = max(width, child.layout->minimumGeometry().width);
      if(child.widget) width = max(width, child.widget->minimumGeometry().width);
      continue;
    }

    width = max(width, child.width);
  }

  foreach(child, children) {
    if(child.height == MaximumSize) {
      maximumHeight = true;
      break;
    }

    if(child.height == MinimumSize) {
      if(child.layout) height += child.layout->minimumGeometry().height;
      if(child.widget) height += child.widget->minimumGeometry().height;
      continue;
    }

    height += child.height;
  }

  return { 0, 0, maximumWidth ? MaximumSize : margin * 2 + width, maximumHeight ? MaximumSize : margin * 2 + height };
}

void VerticalLayout::setGeometry(const Geometry &containerGeometry) {
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

void VerticalLayout::setMargin(unsigned margin) {
  this->margin = margin;
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
