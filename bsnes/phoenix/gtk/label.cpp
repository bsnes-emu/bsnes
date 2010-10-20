void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  object->widget = gtk_label_new(text);
  gtk_misc_set_alignment(GTK_MISC(object->widget), 0.0, 0.0);
  gtk_widget_set_size_request(object->widget, width, height);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}
