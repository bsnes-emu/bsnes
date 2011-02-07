void VerticalLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, children) {
    if(child.layout) child.layout->setParent(parent);
    if(child.widget) Layout::setParent(parent, *child.widget);
  }
}

void VerticalLayout::append(HorizontalLayout &layout, unsigned width, unsigned height, unsigned spacing) {
  children.append({ &layout, 0, width, height, spacing });
}

void VerticalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  children.append({ 0, &widget, width, height, spacing });
}

void VerticalLayout::update(Geometry &geometry) {
  geometry.x += margin;
  geometry.y += margin;
  geometry.width -= margin * 2;
  geometry.height -= margin * 2;

  Geometry baseGeometry = geometry;
  linear_vector<VerticalLayout::Children> children = this->children;

  unsigned minimumHeight = 0;
  foreach(child, children) minimumHeight += child.height + child.spacing;

  unsigned autosizeWidgets = 0;
  foreach(child, children) {
    if(child.height == 0) autosizeWidgets++;
  }
  foreach(child, children) {
    if(child.width == 0) child.width = geometry.width;
    if(child.height == 0) child.height = (geometry.height - minimumHeight) / autosizeWidgets;
  }

  unsigned maxWidth = 0;
  foreach(child, children) {
    maxWidth = max(maxWidth, child.width);
  }

  foreach(child, children) {
    if(child.layout) {
      child.layout->update(geometry);
      geometry.x = baseGeometry.x;
      geometry.width = baseGeometry.width;
      geometry.y += child.spacing;
      geometry.height -= child.spacing;
    }

    if(child.widget) {
      child.widget->setGeometry(geometry.x, geometry.y, child.width, child.height);
      geometry.y += child.height + child.spacing;
      geometry.height -= child.height + child.spacing;
    }
  }

  geometry.x += maxWidth;
  geometry.width -= maxWidth;
}

void VerticalLayout::setMargin(unsigned margin_) {
  margin = margin_;
}

VerticalLayout::VerticalLayout() {
  margin = 0;
}
