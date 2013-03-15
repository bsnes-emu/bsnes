namespace phoenix {

bool pCheckButton::checked() {
  return qtCheckButton->isChecked();
}

Size pCheckButton::minimumSize() {
  Size size = pFont::size(qtWidget->font(), checkButton.state.text);
  return {size.width + 26, size.height + 6};
}

void pCheckButton::setChecked(bool checked) {
  locked = true;
  qtCheckButton->setChecked(checked);
  locked = false;
}

void pCheckButton::setText(const string &text) {
  qtCheckButton->setText(QString::fromUtf8(text));
}

void pCheckButton::constructor() {
  qtWidget = qtCheckButton = new QCheckBox;
  connect(qtCheckButton, SIGNAL(stateChanged(int)), SLOT(onToggle()));

  pWidget::synchronizeState();
  setChecked(checkButton.state.checked);
  setText(checkButton.state.text);
}

void pCheckButton::destructor() {
  delete qtCheckButton;
  qtWidget = qtCheckButton = nullptr;
}

void pCheckButton::orphan() {
  destructor();
  constructor();
}

void pCheckButton::onToggle() {
  checkButton.state.checked = checked();
  if(locked == false && checkButton.onToggle) checkButton.onToggle();
}

}
