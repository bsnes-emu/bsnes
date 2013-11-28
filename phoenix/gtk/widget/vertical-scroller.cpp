namespace phoenix {

static void VerticalScroller_change(GtkRange* gtkRange, VerticalScroller* self) {
  unsigned position = (unsigned)gtk_range_get_value(gtkRange);
  if(self->state.position == position) return;
  self->state.position = position;
  if(!self->p.locked && self->onChange) self->onChange();
}

Size pVerticalScroller::minimumSize() {
  return {20, 0};
}

void pVerticalScroller::setLength(unsigned length) {
  locked = true;
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  locked = false;
}

void pVerticalScroller::setPosition(unsigned position) {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

void pVerticalScroller::constructor() {
  gtkWidget = gtk_vscrollbar_new(0);
  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalScroller_change), (gpointer)&verticalScroller);

  setLength(verticalScroller.state.length);
  setPosition(verticalScroller.state.position);
}

void pVerticalScroller::destructor() {
  gtk_widget_destroy(gtkWidget);
}

void pVerticalScroller::orphan() {
  destructor();
  constructor();
}

}
