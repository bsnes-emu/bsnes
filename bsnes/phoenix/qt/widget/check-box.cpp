bool pCheckBox::checked() {
  return qtCheckBox->isChecked();
}

Geometry pCheckBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), checkBox.state.text);
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

  setChecked(checkBox.state.checked);
  setText(checkBox.state.text);
}

void pCheckBox::destructor() {
  delete qtCheckBox;
  qtWidget = qtCheckBox = 0;
}

void pCheckBox::orphan() {
  destructor();
  constructor();
}

void pCheckBox::onTick() {
  checkBox.state.checked = checked();
  if(locked == false && checkBox.onTick) checkBox.onTick();
}
