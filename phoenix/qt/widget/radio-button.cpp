namespace phoenix {

bool pRadioButton::checked() {
  return qtRadioButton->isChecked();
}

Size pRadioButton::minimumSize() {
  Size size = pFont::size(qtWidget->font(), radioButton.state.text);
  return {size.width + 26, size.height + 6};
}

void pRadioButton::setChecked() {
  parent().locked = true;
  for(auto &item : radioButton.state.group) {
    item.p.qtRadioButton->setChecked(item.state.checked = false);
  }
  qtRadioButton->setChecked(radioButton.state.checked = true);
  parent().locked = false;
}

void pRadioButton::setGroup(const group<RadioButton>& group) {
  parent().locked = true;
  for(auto& item : radioButton.state.group) {
    item.p.qtRadioButton->setChecked(item.state.checked);
  }
  parent().locked = false;
}

void pRadioButton::setText(string text) {
  qtRadioButton->setText(QString::fromUtf8(text));
}

pRadioButton& pRadioButton::parent() {
  if(radioButton.state.group.size()) return radioButton.state.group.first().p;
  return *this;
}

void pRadioButton::constructor() {
  qtWidget = qtRadioButton = new QRadioButton;
  qtRadioButton->setAutoExclusive(false);
  connect(qtRadioButton, SIGNAL(toggled(bool)), SLOT(onActivate()));

  pWidget::synchronizeState();
  setGroup(radioButton.state.group);
  setText(radioButton.state.text);
}

void pRadioButton::destructor() {
  if(qtRadioButton) delete qtRadioButton;
  qtWidget = qtRadioButton = nullptr;
}

void pRadioButton::orphan() {
  destructor();
  constructor();
}

void pRadioButton::onActivate() {
  if(parent().locked == false) {
    bool wasChecked = radioButton.state.checked;
    setChecked();
    if(wasChecked == false) {
      if(radioButton.onActivate) radioButton.onActivate();
    }
  }
}

}
