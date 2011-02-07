void Layout::setParent(Window &parent) {
  layout->parent = &parent;
}

void Layout::append(Widget &child) {
  child.widget->parent = layout->parent;
  gtk_fixed_put(GTK_FIXED(layout->parent->object->formContainer), child.object->widget, 0, 0);
  if(child.widget->font) {
    child.setFont(*child.widget->font);
  } else if(layout->parent->window->defaultFont) {
    child.setFont(*layout->parent->window->defaultFont);
  }
  gtk_widget_show(child.object->widget);
}

Layout::Layout() {
  layout = new Layout::Data;
  layout->parent = 0;
}
