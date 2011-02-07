void ProgressBar::setPosition(unsigned position) {
  progressBar->setValue(position);
}

ProgressBar::ProgressBar() {
  progressBar = new ProgressBar::Data(*this);
  widget->widget = progressBar;
  progressBar->setRange(0, 100);
  progressBar->setTextVisible(false);
}
