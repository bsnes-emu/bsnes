static void VerticalSlider_change(VerticalSlider *self) {
  if(self->object->position == self->position()) return;
  self->object->position = self->position();
  if(self->onChange) self->onChange();
}

unsigned VerticalSlider::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(object->widget));
}

void VerticalSlider::setLength(unsigned length) {
  length += (length == 0);
  gtk_range_set_range(GTK_RANGE(object->widget), 0, length - 1);
}

void VerticalSlider::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(object->widget), position);
}

VerticalSlider::VerticalSlider() {
  object->position = 0;
  object->widget = gtk_vscale_new_with_range(0, 0, 1);
  gtk_scale_set_draw_value(GTK_SCALE(object->widget), false);
  g_signal_connect_swapped(G_OBJECT(object->widget), "value-changed", G_CALLBACK(VerticalSlider_change), (gpointer)this);
}
