namespace hiro {

auto pProgressBar::construct() -> void {
  gtkWidget = gtk_progress_bar_new();

  setPosition(state().position);

  pWidget::construct();
}

auto pProgressBar::destruct() -> void {
  gtk_widget_destroy(gtkWidget);
}

auto pProgressBar::minimumSize() const -> Size {
  return {0, 25};
}

auto pProgressBar::setPosition(unsigned position) -> void {
  position = position <= 100 ? position : 0;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtkWidget), (double)position / 100.0);
}

}
