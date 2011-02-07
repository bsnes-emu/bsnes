void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::setParent(Window &parent, Widget &child) {
  child.widget->parent = &parent;
  gtk_fixed_put(GTK_FIXED(layout->parent->object->formContainer), child.object->widget, 0, 0);
  gtk_widget_show(child.object->widget);
}

Layout::Layout() {
  layout = new Layout::Data;
  layout->parent = 0;
}
