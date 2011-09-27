bool pRadioBox::checked() {
  return qtRadioBox->isChecked();
}

Geometry pRadioBox::minimumGeometry() {
  Geometry geometry = pFont::geometry(qtWidget->font(), radioBox.state.text);
  return { 0, 0, geometry.width + 26, geometry.height + 6 };
}

void pRadioBox::setChecked() {
  locked = true;
  for(auto &item : radioBox.state.group) {
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
  if(group.size() > 0 && qtRadioBox == group[0].p.qtRadioBox) {
    qtGroup = new QButtonGroup;
    for(auto &item : group) qtGroup->addButton(item.p.qtRadioBox);
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

  pWidget::synchronizeState();
  setGroup(radioBox.state.group);
  setText(radioBox.state.text);
}

void pRadioBox::destructor() {
  delete qtGroup;
  delete qtRadioBox;
  qtWidget = qtRadioBox = 0;
  qtGroup = 0;
}

void pRadioBox::orphan() {
  destructor();
  constructor();
}

void pRadioBox::onTick() {
  if(locked == false && checked() && radioBox.onTick) radioBox.onTick();
}
