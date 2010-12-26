void CheckBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const string &text) {
  checkBox->setParent(parent.window->container);
  checkBox->setGeometry(x, y, width, height);
  checkBox->setText(QString::fromUtf8(text));
  if(parent.window->defaultFont) checkBox->setFont(*parent.window->defaultFont);
  checkBox->show();
  checkBox->connect(checkBox, SIGNAL(stateChanged(int)), SLOT(onTick()));
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
}
