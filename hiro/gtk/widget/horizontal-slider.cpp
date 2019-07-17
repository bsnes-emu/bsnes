#if defined(Hiro_HorizontalSlider)

namespace hiro {

static auto HorizontalSlider_change(GtkRange* gtkRange, pHorizontalSlider* p) -> void {
  auto position = (unsigned)gtk_range_get_value(gtkRange);
  if(p->state().position == position) return;
  p->state().position = position;
  if(!p->locked()) p->self().doChange();
}

auto pHorizontalSlider::construct() -> void {
  #if HIRO_GTK==2
  gtkWidget = gtk_hscale_new_with_range(0, 100, 1);
  #elif HIRO_GTK==3
  gtkWidget = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
  #endif
  gtk_scale_set_draw_value(GTK_SCALE(gtkWidget), false);

  setLength(state().length);
  setPosition(state().position);

  g_signal_connect(G_OBJECT(gtkWidget), "value-changed", G_CALLBACK(HorizontalSlider_change), (gpointer)this);

  pWidget::construct();
}

auto pHorizontalSlider::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pHorizontalSlider::minimumSize() const -> Size {
  return {3, 20};
}

auto pHorizontalSlider::setLength(unsigned length) -> void {
  length += length == 0;
  gtk_range_set_range(GTK_RANGE(gtkWidget), 0, max(1u, length - 1));
  gtk_range_set_increments(GTK_RANGE(gtkWidget), 1, length >> 3);
}

auto pHorizontalSlider::setPosition(unsigned position) -> void {
  gtk_range_set_value(GTK_RANGE(gtkWidget), position);
}

}

#endif
