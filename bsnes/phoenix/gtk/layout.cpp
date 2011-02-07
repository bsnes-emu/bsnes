void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::update(Geometry &geometry) {
  geometry.x += layout->margin;
  geometry.y += layout->margin;
  geometry.width -= layout->margin * 2;
  geometry.height -= layout->margin * 2;
}

void Layout::setMargin(unsigned margin) {
  layout->margin = margin;
}

Layout::Layout() {
  layout = new Layout::Data;
  layout->parent = 0;
  layout->margin = 0;
}
