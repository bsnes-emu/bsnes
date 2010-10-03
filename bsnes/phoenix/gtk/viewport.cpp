void Viewport::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  object->widget = gtk_drawing_area_new();
  widget->parent = &parent;
  gtk_widget_set_double_buffered(object->widget, false);
  gtk_widget_set_size_request(object->widget, width, height);

  GdkColor color;
  color.pixel = 0;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  gtk_widget_modify_bg(object->widget, GTK_STATE_NORMAL, &color);

  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

uintptr_t Viewport::handle() {
  return GDK_WINDOW_XID(object->widget->window);
}
