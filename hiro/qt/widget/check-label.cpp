namespace phoenix {

Size pCheckLabel::minimumSize() {
  Size size = pFont::size(qtWidget->font(), checkLabel.state.text);
  return {size.width + 26, size.height + 6};
}

void pCheckLabel::setChecked(bool checked) {
  lock();
  qtCheckLabel->setChecked(checked);
  unlock();
}

void pCheckLabel::setText(string text) {
  qtCheckLabel->setText(QString::fromUtf8(text));
}

void pCheckLabel::constructor() {
  qtWidget = qtCheckLabel = new QCheckBox;
  connect(qtCheckLabel, SIGNAL(stateChanged(int)), SLOT(onToggle()));

  pWidget::synchronizeState();
  setChecked(checkLabel.state.checked);
  setText(checkLabel.state.text);
}

void pCheckLabel::destructor() {
  delete qtCheckLabel;
  qtWidget = qtCheckLabel = nullptr;
}

void pCheckLabel::orphan() {
  destructor();
  constructor();
}

void pCheckLabel::onToggle() {
  checkLabel.state.checked = qtCheckLabel->isChecked();
  if(!locked() && checkLabel.onToggle) checkLabel.onToggle();
}

}
