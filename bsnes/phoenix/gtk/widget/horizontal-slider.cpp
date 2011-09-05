static void HorizontalSlider_change(HorizontalSlider *self) {
  if(self->state.position == self->position()) return;
  self->state.position = self->position();
  if(self->onChange) self->onChange();
}

Geometry pHorizontalSlider::minimumGeometry() {
  return { 0, 0, 0, 20 };
}

unsigned pHorizontalSlider::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(gtkWidget));
}

void pHorizontalSlider::setLength(unsigned length) {
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, length - 1);
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
}

void pHorizontalSlider::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pHorizontalSlider::constructor() {
  gtkWidget = gtk_hscale_new_with_range(0, 100, 1);
  gtk_scale_set_draw_value(GTK_SCALE(gtkWidget), false);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalSlider_change), (gpointer)&horizontalSlider);

  setLength(horizontalSlider.state.length);
  setPosition(horizontalSlider.state.position);
}

void pHorizontalSlider::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pHorizontalSlider::orphan() {
  destructor();
  constructor();
}
