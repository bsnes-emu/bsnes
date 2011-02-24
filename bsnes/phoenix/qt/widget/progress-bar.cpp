void pProgressBar::setPosition(unsigned position) {
  qtProgressBar->setValue(position);
}

void pProgressBar::constructor() {
  qtWidget = qtProgressBar = new QProgressBar;
  qtProgressBar->setRange(0, 100);
  qtProgressBar->setTextVisible(false);
}
