bool pCheckBox::checked() {
  return qtCheckBox->isChecked();
}

Geometry pCheckBox::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(checkBox.state.text);
  return { 0, 0, geometry.width + 26, geometry.height + 6 };
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
