void FixedLayout::setParent(Window &parent) {
  Layout::setParent(parent);
  foreach(child, fixedLayout->children) {
    gtk_fixed_put(GTK_FIXED(layout->parent->object->formContainer), child.widget->object->widget, 0, 0);
  }
}

void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  fixedLayout->children.append({ &widget, x, y, width, height });
}

void FixedLayout::update(Geometry &geometry) {
  Layout::update(geometry);

  foreach(child, fixedLayout->children) {
    gtk_fixed_move(GTK_FIXED(layout->parent->object->formContainer), child.widget->object->widget, child.x, child.y);
    gtk_widget_set_size_request(child.widget->object->widget, child.width, child.height);
    gtk_widget_show(child.widget->object->widget);
  }
}

FixedLayout::FixedLayout() {
  fixedLayout = new FixedLayout::Data;
}
