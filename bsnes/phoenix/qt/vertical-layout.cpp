void VerticalLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, verticalLayout->children) {
    if(child.layout) {
      child.layout->setParent(parent);
    }

    if(child.widget) {
      child.widget->widget->widget->setParent(layout->parent->window->container);
      if(!child.widget->widget->font && layout->parent->window->defaultFont) {
        QWidget *control = child.widget->widget->widget;
        control->setFont(*layout->parent->window->defaultFont);
      }
    }
  }
}

void VerticalLayout::append(HorizontalLayout &layout, unsigned width, unsigned height, unsigned spacing) {
  verticalLayout->children.append({ &layout, 0, width, height, spacing });
}

void VerticalLayout::append(Widget &widget, unsigned width, unsigned height, unsigned spacing) {
  verticalLayout->children.append({ 0, &widget, width, height, spacing });
}

void VerticalLayout::update(Geometry &geometry) {
  Layout::update(geometry);
  Geometry baseGeometry = geometry;
  linear_vector<VerticalLayout::Data::Children> children = verticalLayout->children;

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

VerticalLayout::VerticalLayout() {
  verticalLayout = new VerticalLayout::Data(*this);
}
