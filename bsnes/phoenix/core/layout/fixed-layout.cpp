void FixedLayout::setParent(Window &parent) {
  foreach(child, children) {
    child.sizable->setParent(parent);
    child.sizable->setGeometry(child.geometry);
  }
}

void FixedLayout::append(Sizable &sizable, const Geometry &geometry) {
  children.append({ &sizable, geometry });
}

Geometry FixedLayout::minimumGeometry() {
  unsigned width = MinimumSize, height = MinimumSize;
  foreach(child, children) {
    width  = max(width,  child.sizable->minimumGeometry().width);
    height = max(height, child.sizable->minimumGeometry().height);
  }
  return { 0, 0, width, height };
}

void FixedLayout::setGeometry(const Geometry &geometry) {
}

void FixedLayout::setLayout(Layout &layout) {
  this->layout = &layout;
}

void FixedLayout::setVisible(bool visible) {
  visible_ = visible;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

bool FixedLayout::visible() {
  if(layout) return visible_ && layout->visible();
  return visible_;
}

FixedLayout::FixedLayout() {
  layout = 0;
  parent = 0;
  visible_ = true;
}
