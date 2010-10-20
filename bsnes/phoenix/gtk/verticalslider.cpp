static void VerticalSlider_change(VerticalSlider *self) {
  if(self->object->position == self->position()) return;
  self->object->position = self->position();
  if(self->onChange) self->onChange();
}

void VerticalSlider::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length) {
  object->position = 0;
  length += (length == 0);
  object->widget = gtk_vscale_new_with_range(0, length - 1, 1);
  gtk_scale_set_draw_value(GTK_SCALE(object->widget), false);
  gtk_widget_set_size_request(object->widget, width, height);
  g_signal_connect_swapped(G_OBJECT(object->widget), "value-changed", G_CALLBACK(VerticalSlider_change), (gpointer)this);
  gtk_fixed_put(GTK_FIXED(parent.object->formContainer), object->widget, x, y);
  gtk_widget_show(object->widget);
}

unsigned VerticalSlider::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(object->widget));
}

void VerticalSlider::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(object->widget), position);
}
