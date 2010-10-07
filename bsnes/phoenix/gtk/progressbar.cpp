void ProgressBar::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  object->widget = gtk_progress_bar_new();
  widget->parent = &parent;
  gtk_widget_set_size_request(object->widget, width, height);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

void ProgressBar::setPosition(unsigned position) {
  position = position <= 100 ? position : 0;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(object->widget), (double)position / 100.0);
}
