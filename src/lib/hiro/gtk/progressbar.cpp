void pProgressbar::create(unsigned style, unsigned width, unsigned height) {
  progressbar = gtk_progress_bar_new();
  gtk_widget_set_size_request(progressbar, width, height);
  gtk_widget_show(progressbar);
}

unsigned pProgressbar::get_progress() {
  unsigned progress = (unsigned)(gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar)) * 100.0);
  return max(0U, min(progress, 100U));
}

void pProgressbar::set_progress(unsigned progress) {
  progress = max(0U, min(progress, 100U));
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), (double)progress / 100.0);
}

pProgressbar::pProgressbar(Progressbar &self_) : pFormControl(self_), self(self_) {
  progressbar = 0;
}

/* internal */

GtkWidget* pProgressbar::gtk_handle() {
  return progressbar;
}
