void FixedLayout::setParent(Window &parent) {
  foreach(child, children) {
    parent.append(*child.widget);
    child.widget->setGeometry(child.geometry);
  }
}

void FixedLayout::append(Widget &widget, const Geometry &geometry) {
  children.append({ &widget, geometry });
}

void FixedLayout::setGeometry(const Geometry &geometry) {
}

void FixedLayout::setVisible(bool visible) {
  foreach(child, children) child.widget->setVisible(visible);
}

FixedLayout::FixedLayout() {
}
