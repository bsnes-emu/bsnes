void VerticalLayout::append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing) {
  sizable.setLayout(*this);
  children.append({ &sizable, width, height, spacing });
}

Geometry VerticalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  foreach(child, children) {
    if(child.width == MinimumSize || child.width == MaximumSize) {
      width = max(width, child.sizable->minimumGeometry().width);
      continue;
    }
    width = max(width, child.width);
  }

  foreach(child, children) {
    height += child.spacing;
    if(child.height == MinimumSize || child.height == MaximumSize) {
      height += child.sizable->minimumGeometry().height;
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
      width = max(width, child.sizable->minimumGeometry().width);
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
      height += child.sizable->minimumGeometry().height;
      continue;
    }

    height += child.height;
  }

  return { 0, 0, maximumWidth ? MaximumSize : margin * 2 + width, maximumHeight ? MaximumSize : margin * 2 + height };
}

void VerticalLayout::setAlignment(double alignment) {
  this->alignment = max(0.0, min(1.0, alignment));
}

void VerticalLayout::setGeometry(const Geometry &containerGeometry) {
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
    unsigned pivot = (maximumWidth - child.width) * alignment;
    Geometry childGeometry = { geometry.x + pivot, geometry.y, child.width, child.height };
    child.sizable->setGeometry(childGeometry);

    geometry.y += child.height + child.spacing;
    geometry.height -= child.height + child.spacing;
  }
}

void VerticalLayout::setLayout(Layout &layout) {
  this->layout = &layout;
}

void VerticalLayout::setMargin(unsigned margin) {
  this->margin = margin;
}

void VerticalLayout::setParent(Window &parent) {
  foreach(child, children) {
    child.sizable->setParent(parent);
  }
}

void VerticalLayout::setVisible(bool visible) {
  visible_ = visible;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

bool VerticalLayout::visible() {
  if(layout) return visible_ && layout->visible();
  return visible_;
}

VerticalLayout::VerticalLayout() {
  alignment = 0.0;
  layout = 0;
  margin = 0;
  visible_ = true;
}
