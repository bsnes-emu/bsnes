bool pRadioBox::checked() {
  return qtRadioBox->isChecked();
}

Geometry pRadioBox::minimumGeometry() {
  Font &font = this->font();
  Geometry geometry = font.geometry(radioBox.state.text);
  return { 0, 0, geometry.width + 26, geometry.height + 6 };
}

void pRadioBox::setChecked() {
  locked = true;
  foreach(item, radioBox.state.group) {
    bool checkState = item.p.qtRadioBox == qtRadioBox;
    item.state.checked = checkState;
    item.p.qtRadioBox->setChecked(checkState);
  }
  locked = false;
}

void pRadioBox::setGroup(const reference_array<RadioBox&> &group) {
  locked = true;
  if(qtGroup) {
    delete qtGroup;
    qtGroup = 0;
  }
  if(qtRadioBox == group[0].p.qtRadioBox) {
    qtGroup = new QButtonGroup;
    foreach(item, group) qtGroup->addButton(item.p.qtRadioBox);
    setChecked();
  }
  locked = false;
}

void pRadioBox::setText(const string &text) {
  qtRadioBox->setText(QString::fromUtf8(text));
}

void pRadioBox::constructor() {
  qtWidget = qtRadioBox = new QRadioButton;
  qtGroup = new QButtonGroup;
  qtGroup->addButton(qtRadioBox);
  qtRadioBox->setChecked(true);
  connect(qtRadioBox, SIGNAL(toggled(bool)), SLOT(onTick()));
}

void pRadioBox::onTick() {
  if(locked == false && checked() && radioBox.onTick) radioBox.onTick();
}
