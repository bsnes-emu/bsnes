#if defined(Hiro_VerticalSlider)

namespace hiro {

static auto VerticalSlider_change(GtkRange* gtkRange, pVerticalSlider* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pVerticalSlider::construct() -> void {
  #if HIRO_GTK==2
  gtkWidget = gtk_vscale_new_with_range(0, 100, 1);
  #elif HIRO_GTK==3
  gtkWidget = gtk_scale_new_with_range(GTK_ORIENTATION_VERTICAL, 0, 100, 1);
  #endif
  gtk_scale_set_draw_value(GTK_SCALE(gtkWidget), false);

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(VerticalSlider_change), (gpointer)this);

  pWidget::construct();
}

auto pVerticalSlider::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pVerticalSlider::minimumSize() const -> Size {
  return {20, 0};
}

auto pVerticalSlider::setLength(unsigned length) -> void {
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
}

auto pVerticalSlider::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}

#endif
