void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::setParent(Window &parent, Widget &child) {
  child.widget->widget->setParent(layout->parent->window->container);
  if(!child.widget->font && layout->parent->window->defaultFont) {
    QWidget *control = child.widget->widget;
    control->setFont(*layout->parent->window->defaultFont);
  }
}

Layout::Layout() {
  layout = new Layout::Data(*this);
}
