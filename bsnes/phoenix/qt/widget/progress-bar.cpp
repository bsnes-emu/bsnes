Geometry pProgressBar::minimumGeometry() {
  return { 0, 0, 0, 25 };
}

void pProgressBar::setPosition(unsigned position) {
  qtProgressBar->setValue(position);
}

void pProgressBar::constructor() {
  qtWidget = qtProgressBar = new QProgressBar;
  qtProgressBar->setRange(0, 100);
  qtProgressBar->setTextVisible(false);

  pWidget::synchronizeState();
  setPosition(progressBar.state.position);
}

void pProgressBar::destructor() {
  delete qtProgressBar;
  qtWidget = qtProgressBar = 0;
}

void pProgressBar::orphan() {
  destructor();
  constructor();
}
