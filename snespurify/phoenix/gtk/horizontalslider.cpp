static void HorizontalSlider_change(HorizontalSlider *self) {
  if(self->object->position == self->position()) return;
  self->object->position = self->position();
  if(self->onChange) self->onChange();
}

void HorizontalSlider::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length) {
  object->position = 0;
  length += (length == 0);
  object->widget = gtk_hscale_new_with_range(0, length - 1, 1);
  widget->parent = &parent;
  gtk_scale_set_draw_value(GTK_SCALE(object->widget), false);
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "value-changed", G_CALLBACK(HorizontalSlider_change), (gpointer)this);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

unsigned HorizontalSlider::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(object->widget));
}

void HorizontalSlider::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(object->widget), position);
}
