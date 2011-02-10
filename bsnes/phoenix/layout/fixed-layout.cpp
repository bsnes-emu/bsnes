void FixedLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, children) {
    Layout::append(*child.widget);
    child.widget->setGeometry(child.x, child.y, child.width, child.height);
  }
}

void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  children.append({ &widget, x, y, width, height });
}

void FixedLayout::setGeometry(Geometry &geometry) {
}

FixedLayout::FixedLayout() {
}
