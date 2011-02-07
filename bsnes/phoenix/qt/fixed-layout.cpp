void FixedLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, fixedLayout->children) {
    child.widget->widget->widget->setParent(layout->parent->window->container);
    if(!child.widget->widget->font && layout->parent->window->defaultFont) {
      QWidget *control = child.widget->widget->widget;
      control->setFont(*layout->parent->window->defaultFont);
    }
  }
}

void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  fixedLayout->children.append({ &widget, x, y, width, height });
}

void FixedLayout::update(Geometry &geometry) {
  Layout::update(geometry);

  foreach(child, fixedLayout->children) {
    child.widget->widget->widget->setParent(layout->parent->window->container);
    child.widget->setGeometry(child.x, child.y, child.width, child.height);
    if(layout->parent->window->defaultFont) {
      QWidget *control = child.widget->widget->widget;
      control->setFont(*layout->parent->window->defaultFont);
    }
  }
}

FixedLayout::FixedLayout() {
  fixedLayout = new FixedLayout::Data(*this);
}
