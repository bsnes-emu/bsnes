#if defined(Hiro_HorizontalScroller)

namespace hiro {

static auto HorizontalScroller_change(GtkRange* gtkRange, pHorizontalScroller* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pHorizontalScroller::construct() -> void {
  gtkWidget = gtk_hscrollbar_new(0);

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalScroller_change), (gpointer)this);

  pWidget::construct();
}

auto pHorizontalScroller::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pHorizontalScroller::minimumSize() const -> Size {
  return {0, 20};
}

auto pHorizontalScroller::setLength(unsigned length) -> void {
  lock();
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
  unlock();
}

auto pHorizontalScroller::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}

#endif
