void HorizontalLayout::append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing) {
  sizable.setLayout(*this);
  children.append({ &sizable, width, height, spacing });
}

Geometry HorizontalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  foreach(child, children) {
    width += child.spacing;
    if(child.width == MinimumSize || child.width == MaximumSize) {
      width += child.sizable->minimumGeometry().width;
      continue;
    }
    width += child.width;
  }

  foreach(child, children) {
    if(child.height == MinimumSize || child.height == MaximumSize) {
      height = max(height, child.sizable->minimumGeometry().height);
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
      width += child.sizable->minimumGeometry().width;
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
      height = max(height, child.sizable->minimumGeometry().height);
      continue;
    }

    height = max(height, child.height);
  }

  return { 0, 0, maximumWidth ? MaximumSize : margin * 2 + width, maximumHeight ? MaximumSize : margin * 2 + height };
}

void HorizontalLayout::setAlignment(double alignment) {
  this->alignment = max(0.0, min(1.0, alignment));
}

void HorizontalLayout::setGeometry(const Geometry &containerGeometry) {
  auto children = this->children;
  foreach(child, children) {
    if(child.width  == MinimumSize) child.width  = child.sizable->minimumGeometry().width;
    if(child.height == MinimumSize) child.height = child.sizable->minimumGeometry().height;
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
    unsigned pivot = (maximumHeight - child.height) * alignment;
    Geometry childGeometry = { geometry.x, geometry.y + pivot, child.width, child.height };
    child.sizable->setGeometry(childGeometry);

    geometry.x += child.width + child.spacing;
    geometry.width -= child.width + child.spacing;
  }
}

void HorizontalLayout::setLayout(Layout &layout) {
  this->layout = &layout;
}

void HorizontalLayout::setMargin(unsigned margin) {
  this->margin = margin;
}

void HorizontalLayout::setParent(Window &parent) {
  foreach(child, children) {
    child.sizable->setParent(parent);
  }
}

void HorizontalLayout::setVisible(bool visible) {
  visible_ = visible;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

bool HorizontalLayout::visible() {
  if(layout) return visible_ && layout->visible();
  return visible_;
}

HorizontalLayout::HorizontalLayout() {
  alignment = 0.5;
  layout = 0;
  margin = 0;
  visible_ = true;
}
