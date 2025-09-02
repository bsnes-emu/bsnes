#if defined(Hiro_HorizontalScrollBar)

namespace hiro {

static auto HorizontalScrollBar_change(GtkRange* gtkRange, pHorizontalScrollBar* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pHorizontalScrollBar::construct() -> void {
  gtkWidget = gtk_scrollbar_new(GTK_ORIENTATION_HORIZONTAL, nullptr);

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalScrollBar_change), (gpointer)this);

  pWidget::construct();
}

auto pHorizontalScrollBar::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pHorizontalScrollBar::minimumSize() const -> Size {
  return {0, 20};
}

auto pHorizontalScrollBar::setLength(unsigned length) -> void {
  auto lock = acquire();
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
}

auto pHorizontalScrollBar::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}

#endif
