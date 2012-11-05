static void VerticalScrollBar_change(VerticalScrollBar *self) {
  if(self->state.position == self->position()) return;
  self->state.position = self->position();
  if(self->p.locked == false && self->onChange) self->onChange();
}

Geometry pVerticalScrollBar::minimumGeometry() {
  return { 0, 0, 20, 0 };
}

unsigned pVerticalScrollBar::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(gtkWidget));
}

void pVerticalScrollBar::setLength(unsigned length) {
  locked = true;
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  locked = false;
}

void pVerticalScrollBar::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pVerticalScrollBar::constructor() {
  gtkWidget = gtk_vscrollbar_new(0);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalScrollBar_change), (gpointer)&verticalScrollBar);

  setLength(verticalScrollBar.state.length);
  setPosition(verticalScrollBar.state.position);
}

void pVerticalScrollBar::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pVerticalScrollBar::orphan() {
  destructor();
  constructor();
}
