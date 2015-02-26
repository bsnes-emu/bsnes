namespace phoenix {

Size pRadioLabel::minimumSize() {
  Size size = pFont::size(qtWidget->font(), radioLabel.state.text);
  return {size.width + 26, size.height + 6};
}

void pRadioLabel::setChecked() {
  parent().lock();
  for(auto& item : radioLabel.state.group) {
    bool checked = &item == &radioLabel;
    item.p.qtRadioLabel->setChecked(item.state.checked = checked);
  }
  parent().unlock();
}

void pRadioLabel::setGroup(const group<RadioLabel>& group) {
  parent().lock();
  for(auto& item : radioLabel.state.group) {
    item.p.qtRadioLabel->setChecked(item.state.checked);
  }
  parent().unlock();
}

void pRadioLabel::setText(string text) {
  qtRadioLabel->setText(QString::fromUtf8(text));
}

pRadioLabel& pRadioLabel::parent() {
  if(radioLabel.state.group.size()) return radioLabel.state.group.first().p;
  return *this;
}

void pRadioLabel::constructor() {
  qtWidget = qtRadioLabel = new QRadioButton;
  qtRadioLabel->setAutoExclusive(false);
  connect(qtRadioLabel, SIGNAL(toggled(bool)), SLOT(onActivate()));

  pWidget::synchronizeState();
  setGroup(radioLabel.state.group);
  setText(radioLabel.state.text);
}

void pRadioLabel::destructor() {
  if(qtRadioLabel) delete qtRadioLabel;
  qtWidget = qtRadioLabel = nullptr;
}

void pRadioLabel::orphan() {
  destructor();
  constructor();
}

void pRadioLabel::onActivate() {
  if(parent().locked()) return;
  bool wasChecked = radioLabel.state.checked;
  setChecked();
  if(!wasChecked) {
    if(radioLabel.onActivate) radioLabel.onActivate();
  }
}

}
