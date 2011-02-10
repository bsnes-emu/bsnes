uintptr_t Viewport::handle() {
  return GDK_WINDOW_XID(object->widget->window);
}

Viewport::Viewport() {
  object->widget = gtk_drawing_area_new();
//gtk_widget_set_double_buffered(object->widget, false);

  GdkColor color;
  color.pixel = 0;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  gtk_widget_modify_bg(object->widget, GTK_STATE_NORMAL, &color);
}
