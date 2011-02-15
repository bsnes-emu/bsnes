void FixedLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, children) {
    Layout::append(*child.widget);
    child.widget->setGeometry(child.geometry);
  }
}

void FixedLayout::append(Widget &widget, const Geometry &geometry) {
  children.append({ &widget, geometry });
}

void FixedLayout::setGeometry(Geometry &geometry) {
}

FixedLayout::FixedLayout() {
}
