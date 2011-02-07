void HorizontalLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, children) {
    if(child.layout) child.layout->setParent(parent);
    if(child.widget) Layout::append(*child.widget);
  }
}

void HorizontalLayout::append(VerticalLayout &layout, unsigned width, unsigned height, unsigned spacing) {
  children.append({ &layout, 0, width, height, spacing });
}

void HorizontalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

void HorizontalLayout::update(Geometry &geometry) {
  geometry.x += margin;
  geometry.y += margin;
  geometry.width -= margin * 2;
  geometry.height -= margin * 2;

  Geometry baseGeometry = geometry;
  linear_vector<HorizontalLayout::Children> children = this->children;

  unsigned minimumWidth = 0;
  foreach(child, children) minimumWidth += child.width + child.spacing;

  unsigned autosizeWidgets = 0;
  foreach(child, children) {
    if(child.width == 0) autosizeWidgets++;
  }
  foreach(child, children) {
    if(child.width == 0) child.width = (geometry.width - minimumWidth) / autosizeWidgets;
    if(child.height == 0) child.height = geometry.height;
  }

  unsigned maxHeight = 0;
  foreach(child, children) {
    maxHeight = max(maxHeight, child.height);
  }

  foreach(child, children) {
    if(child.layout) {
      child.layout->update(geometry);
      geometry.x += child.spacing;
      geometry.width -= child.spacing;
      geometry.y = baseGeometry.y;
      geometry.height = baseGeometry.height;
    }

    if(child.widget) {
      child.widget->setGeometry(geometry.x, geometry.y, child.width, child.height);
      geometry.x += child.width + child.spacing;
      geometry.width -= child.width + child.spacing;
    }
  }

  geometry.y += maxHeight;
  geometry.height -= maxHeight;
}

void HorizontalLayout::setMargin(unsigned margin_) {
  margin = margin_;
}

HorizontalLayout::HorizontalLayout() {
  margin = 0;
}
