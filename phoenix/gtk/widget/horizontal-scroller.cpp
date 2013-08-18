namespace phoenix {

static void HorizontalScroller_change(HorizontalScroller* self) {
  if(self->state.position == self->position()) return;
  self->state.position = self->position();
  if(self->p.locked == false && self->onChange) self->onChange();
}

Size pHorizontalScroller::minimumSize() {
  return {0, 20};
}

unsigned pHorizontalScroller::position() {
  return (unsigned)gtk_range_get_value(GTK_RANGE(gtkWidget));
}

void pHorizontalScroller::setLength(unsigned length) {
  locked = true;
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  locked = false;
}

void pHorizontalScroller::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pHorizontalScroller::constructor() {
  gtkWidget = gtk_hscrollbar_new(0);
  g_signal_connect_swapped(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalScroller_change), (gpointer)&horizontalScroller);

  setLength(horizontalScroller.state.length);
  setPosition(horizontalScroller.state.position);
}

void pHorizontalScroller::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pHorizontalScroller::orphan() {
  destructor();
  constructor();
}

}
