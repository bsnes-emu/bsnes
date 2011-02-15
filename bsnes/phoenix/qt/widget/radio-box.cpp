bool pRadioBox::checked() {
  return qtRadioBox->isChecked();
}

void pRadioBox::setChecked() {
  locked = true;
  foreach(item, radioBox.state.group) {
    bool checkState = item->p.qtRadioBox == qtRadioBox;
    item->state.checked = checkState;
    item->p.qtRadioBox->setChecked(checkState);
  }
  locked = false;
}

void pRadioBox::setGroup(const array<RadioBox*> &group) {
  locked = true;
  if(qtGroup) {
    delete qtGroup;
    qtGroup = 0;
  }
  if(qtRadioBox == group[0]->p.qtRadioBox) {
    qtGroup = new QButtonGroup;
    foreach(item, group) qtGroup->addButton(item->p.qtRadioBox);
    setChecked();
  }
  locked = false;
}

void pRadioBox::setText(const string &text) {
  qtRadioBox->setText(QString::fromUtf8(text));
}

pRadioBox::pRadioBox(RadioBox &radioBox) : radioBox(radioBox), pWidget(radioBox) {
  qtWidget = qtRadioBox = new QRadioButton;
  qtGroup = new QButtonGroup;
  qtGroup->addButton(qtRadioBox);
  qtRadioBox->setChecked(true);
  connect(qtRadioBox, SIGNAL(toggled(bool)), SLOT(onTick()));
}

void pRadioBox::onTick() {
  if(locked == false && checked() && radioBox.onTick) radioBox.onTick();
}
