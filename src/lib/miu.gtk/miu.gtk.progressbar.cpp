void pProgressbar::create(uint style, uint width, uint height) {
  progressbar = gtk_progress_bar_new();
  gtk_widget_set_size_request(progressbar, width, height);
  gtk_widget_show(progressbar);
}

uint pProgressbar::get_progress() {
uint progress = (uint)(gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbar)) * 100.0);
  return max(0, min(progress, 100));
}

void pProgressbar::set_progress(uint progress) {
  progress = max(0, min(progress, 100));
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), (double)progress / 100.0);
}

pProgressbar::pProgressbar(Progressbar &self_) : pFormControl(self_), self(self_) {
  progressbar = 0;
}

/* internal */

GtkWidget* pProgressbar::gtk_handle() {
  return progressbar;
}
