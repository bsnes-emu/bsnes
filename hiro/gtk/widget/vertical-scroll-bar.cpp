#if defined(Hiro_VerticalScrollBar)

namespace hiro {

static auto VerticalScrollBar_change(GtkRange* gtkRange, pVerticalScrollBar* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pVerticalScrollBar::construct() -> void {
  #if HIRO_GTK==2
  gtkWidget = gtk_vscrollbar_new(nullptr);
  #elif HIRO_GTK==3
  gtkWidget = gtk_scrollbar_new(GTK_ORIENTATION_VERTICAL, nullptr);
  #endif

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalScrollBar_change), (gpointer)this);

  pWidget::construct();
}

auto pVerticalScrollBar::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pVerticalScrollBar::minimumSize() const -> Size {
  return {20, 0};
}

auto pVerticalScrollBar::setLength(unsigned length) -> void {
  auto lock = acquire();
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
}

auto pVerticalScrollBar::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}

#endif
