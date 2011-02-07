void FixedLayout::append(Widget &widget, unsigned x, unsigned y, unsigned width, unsigned height) {
  fixedLayout->widgets.append({ &widget, x, y, width, height });
}

void FixedLayout::create(Window &parent) {
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), fixedLayout->container, 0, 0);
  gtk_widget_set_size_request(fixedLayout->container, 640, 480);

  foreach(widget, fixedLayout->widgets) {
    gtk_widget_set_size_request(widget.widget->object->widget, widget.width, widget.height);
    gtk_fixed_put(GTK_FIXED(fixedLayout->container), widget.widget->object->widget, widget.x, widget.y);
    gtk_widget_show(widget.widget->object->widget);
  }

  gtk_widget_show(fixedLayout->container);
}

FixedLayout::FixedLayout() {
  fixedLayout = new FixedLayout::Data;
  fixedLayout->container = gtk_fixed_new();
}
