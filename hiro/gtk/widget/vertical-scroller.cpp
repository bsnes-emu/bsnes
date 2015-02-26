namespace hiro {

static auto VerticalScroller_change(GtkRange* gtkRange, pVerticalScroller* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pVerticalScroller::construct() -> void {
  gtkWidget = gtk_vscrollbar_new(0);

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalScroller_change), (gpointer)this);

  pWidget::construct();
}

auto pVerticalScroller::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pVerticalScroller::minimumSize() const -> Size {
  return {20, 0};
}

auto pVerticalScroller::setLength(unsigned length) -> void {
  lock();
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  unlock();
}

auto pVerticalScroller::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}
