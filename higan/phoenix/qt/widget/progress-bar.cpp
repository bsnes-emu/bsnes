namespace phoenix {

Size pProgressBar::minimumSize() {
  return {0, 25};
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
  qtWidget = qtProgressBar = nullptr;
}

void pProgressBar::orphan() {
  destructor();
  constructor();
}

}
