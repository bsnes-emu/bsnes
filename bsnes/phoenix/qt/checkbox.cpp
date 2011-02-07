void CheckBox::setParent(Layout &parent) {
  checkBox->setParent(parent.widget->widget);
  checkBox->show();
}

void CheckBox::setText(const string &text) {
  checkBox->setText(QString::fromUtf8(text));
}

bool CheckBox::checked() {
  return checkBox->isChecked();
}

void CheckBox::setChecked(bool checked) {
  checkBox->setChecked(checked);
}

CheckBox::CheckBox() {
  checkBox = new CheckBox::Data(*this);
  widget->widget = checkBox;
  checkBox->connect(checkBox, SIGNAL(stateChanged(int)), SLOT(onTick()));
}
