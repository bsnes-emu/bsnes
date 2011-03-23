Geometry pProgressBar::minimumGeometry() {
  return { 0, 0, 0, 25 };
}

void pProgressBar::setPosition(unsigned position) {
  position = position <= 100 ? position : 0;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gtkWidget), (double)position / 100.0);
}

void pProgressBar::constructor() {
  gtkWidget = gtk_progress_bar_new();
}
