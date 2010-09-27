void Label::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  object->widget = gtk_label_new(text);
  widget->parent = &parent;
  gtk_misc_set_alignment(GTK_MISC(object->widget), 0.0, 0.5);
  gtk_widget_set_size_request(object->widget, width, height);
  if(parent.window->defaultFont) setFont(*parent.window->defaultFont);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

void Label::setText(const char *text) {
  gtk_label_set_text(GTK_LABEL(object->widget), text);
}
