void ProgressBar::setPosition(unsigned position) {
  position = position <= 100 ? position : 0;
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(object->widget), (double)position / 100.0);
}

ProgressBar::ProgressBar() {
  object->widget = gtk_progress_bar_new();
}
