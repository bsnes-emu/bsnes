void pProgressBar::setPosition(unsigned position) {
  qtProgressBar->setValue(position);
}

pProgressBar::pProgressBar(ProgressBar &progressBar) : progressBar(progressBar), pWidget(progressBar) {
  qtWidget = qtProgressBar = new QProgressBar;
  qtProgressBar->setRange(0, 100);
  qtProgressBar->setTextVisible(false);
}
