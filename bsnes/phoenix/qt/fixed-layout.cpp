void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  fixedLayout->children.append({ &widget, x, y, width, height });
}

void FixedLayout::create(Window &parentWindow) {
  fixedLayout->parent = &parentWindow;
  fixedLayout->setParent(parentWindow.window->container);

  foreach(child, fixedLayout->children) {
    child.widget->setParent(*this);
    child.widget->setGeometry(child.x, child.y, child.width, child.height);
    if(parentWindow.window->defaultFont) {
      QWidget *control = child.widget->widget->widget;
      control->setFont(*parentWindow.window->defaultFont);
    }
  }
}

FixedLayout::FixedLayout() {
  fixedLayout = new FixedLayout::Data(*this);
  widget->widget = fixedLayout;
}
