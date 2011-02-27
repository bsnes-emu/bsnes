bool pCheckBox::checked() {
  return qtCheckBox->isChecked();
}

void pCheckBox::setChecked(bool checked) {
  locked = true;
  qtCheckBox->setChecked(checked);
  locked = false;
}

void pCheckBox::setText(const string &text) {
  qtCheckBox->setText(QString::fromUtf8(text));
}

void pCheckBox::constructor() {
  qtWidget = qtCheckBox = new QCheckBox;
  connect(qtCheckBox, SIGNAL(stateChanged(int)), SLOT(onTick()));
}

void pCheckBox::onTick() {
  if(locked == false && checkBox.onTick) checkBox.onTick();
}
