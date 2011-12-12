static void HorizontalScrollBar_change(HorizontalScrollBar *self) {
  if(self->state.position == self->position()) return;
  self->state.position = self->position();
  if(self->p.locked == false && self->onChange) self->onChange();
}

Geometry pHorizontalScrollBar::minimumGeometry() {
  return { 0, 0, 0, 20 };
}

unsigned pHorizontalScrollBar::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(gtkWidget));
}

void pHorizontalScrollBar::setLength(unsigned length) {
  locked = true;
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, length - 1);
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  locked = false;
}

void pHorizontalScrollBar::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pHorizontalScrollBar::constructor() {
  gtkWidget = gtk_hscrollbar_new(0);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalScrollBar_change), (gpointer)&horizontalScrollBar);

  setLength(horizontalScrollBar.state.length);
  setPosition(horizontalScrollBar.state.position);
}

void pHorizontalScrollBar::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pHorizontalScrollBar::orphan() {
  destructor();
  constructor();
}
