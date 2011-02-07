void RadioBox::setParent(RadioBox &parent) {
  parent.radioBox->buttonGroup->addButton(radioBox);
  parent.radioBox->setChecked(true);
}

void RadioBox::setText(const string &text) {
  radioBox->setText(QString::fromUtf8(text));
}

bool RadioBox::checked() {
  return radioBox->isChecked();
}

void RadioBox::setChecked() {
  radioBox->setChecked(true);
}

RadioBox::RadioBox() {
  radioBox = new RadioBox::Data(*this);
  widget->widget = radioBox;
  radioBox->buttonGroup = new QButtonGroup;
  radioBox->buttonGroup->addButton(radioBox);
  radioBox->setChecked(true);
  radioBox->connect(radioBox, SIGNAL(toggled(bool)), SLOT(onTick()));
}
