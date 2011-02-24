static void VerticalSlider_change(VerticalSlider *self) {
  if(self->state.position == self->position()) return;
  self->state.position = self->position();
  if(self->onChange) self->onChange();
}

unsigned pVerticalSlider::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(gtkWidget));
}

void pVerticalSlider::setLength(unsigned length) {
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, length - 1);
}

void pVerticalSlider::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pVerticalSlider::constructor() {
  gtkWidget = gtk_vscale_new_with_range(0, 100, 1);
  gtk_scale_set_draw_value(GTK_SCALE(gtkWidget), false);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalSlider_change), (gpointer)&verticalSlider);
}
